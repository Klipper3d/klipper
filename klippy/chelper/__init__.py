# Wrapper around C helper code
#
# Copyright (C) 2016-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, logging
import cffi


######################################################################
# c_helper.so compiling
######################################################################

GCC_CMD = "gcc"
COMPILE_ARGS = ("-Wall -g -O2 -shared -fPIC"
                " -flto -fwhole-program -fno-use-linker-plugin"
                " -o %s %s")
SSE_FLAGS = "-mfpmath=sse -msse2"
SOURCE_FILES = [
    'pyhelper.c', 'serialqueue.c', 'stepcompress.c', 'steppersync.c',
    'itersolve.c', 'trapq.c', 'pollreactor.c', 'msgblock.c', 'trdispatch.c',
    'kin_cartesian.c', 'kin_corexy.c', 'kin_corexz.c', 'kin_delta.c',
    'kin_deltesian.c', 'kin_polar.c', 'kin_rotary_delta.c', 'kin_winch.c',
    'kin_extruder.c', 'kin_shaper.c', 'kin_idex.c', 'kin_generic.c'
]
DEST_LIB = "c_helper.so"
OTHER_FILES = [
    'list.h', 'serialqueue.h', 'stepcompress.h', 'steppersync.h',
    'itersolve.h', 'pyhelper.h', 'trapq.h', 'pollreactor.h', 'msgblock.h'
]

defs_stepcompress = """
    struct pull_history_steps {
        uint64_t first_clock, last_clock;
        int64_t start_position;
        int step_count, interval, add;
    };

    void stepcompress_fill(struct stepcompress *sc, uint32_t oid
        , uint32_t max_error, int32_t queue_step_msgtag
        , int32_t set_next_step_dir_msgtag);
    void stepcompress_set_invert_sdir(struct stepcompress *sc
        , uint32_t invert_sdir);
    int stepcompress_reset(struct stepcompress *sc, uint64_t last_step_clock);
    int stepcompress_set_last_position(struct stepcompress *sc
        , uint64_t clock, int64_t last_position);
    int64_t stepcompress_find_past_position(struct stepcompress *sc
        , uint64_t clock);
    int stepcompress_extract_old(struct stepcompress *sc
        , struct pull_history_steps *p, int max
        , uint64_t start_clock, uint64_t end_clock);
"""

defs_steppersync = """
    struct stepcompress *syncemitter_get_stepcompress(struct syncemitter *se);
    void syncemitter_set_stepper_kinematics(struct syncemitter *se
        , struct stepper_kinematics *sk);
    struct stepper_kinematics *syncemitter_get_stepper_kinematics(
        struct syncemitter *se);
    void syncemitter_queue_msg(struct syncemitter *se, uint64_t req_clock
        , uint32_t *data, int len);
    struct syncemitter *steppersync_alloc_syncemitter(struct steppersync *ss
        , char name[16], int alloc_stepcompress);
    void steppersync_setup_movequeue(struct steppersync *ss
        , struct serialqueue *sq, int move_num);
    void steppersync_set_time(struct steppersync *ss
        , double time_offset, double mcu_freq);
    struct steppersyncmgr *steppersyncmgr_alloc(void);
    void steppersyncmgr_free(struct steppersyncmgr *ssm);
    struct steppersync *steppersyncmgr_alloc_steppersync(
        struct steppersyncmgr *ssm);
    int32_t steppersyncmgr_gen_steps(struct steppersyncmgr *ssm
        , double flush_time, double gen_steps_time, double clear_history_time);
"""

defs_itersolve = """
    double itersolve_check_active(struct stepper_kinematics *sk
        , double flush_time);
    int32_t itersolve_is_active_axis(struct stepper_kinematics *sk, char axis);
    void itersolve_set_trapq(struct stepper_kinematics *sk, struct trapq *tq
        , double step_dist);
    struct trapq *itersolve_get_trapq(struct stepper_kinematics *sk);
    double itersolve_calc_position_from_coord(struct stepper_kinematics *sk
        , double x, double y, double z);
    void itersolve_set_position(struct stepper_kinematics *sk
        , double x, double y, double z);
    double itersolve_get_commanded_pos(struct stepper_kinematics *sk);
    double itersolve_get_gen_steps_pre_active(struct stepper_kinematics *sk);
    double itersolve_get_gen_steps_post_active(struct stepper_kinematics *sk);
"""

defs_trapq = """
    struct pull_move {
        double print_time, move_t;
        double start_v, accel;
        double start_x, start_y, start_z;
        double x_r, y_r, z_r;
    };

    struct trapq *trapq_alloc(void);
    void trapq_free(struct trapq *tq);
    void trapq_append(struct trapq *tq, double print_time
        , double accel_t, double cruise_t, double decel_t
        , double start_pos_x, double start_pos_y, double start_pos_z
        , double axes_r_x, double axes_r_y, double axes_r_z
        , double start_v, double cruise_v, double accel);
    void trapq_finalize_moves(struct trapq *tq, double print_time
        , double clear_history_time);
    void trapq_set_position(struct trapq *tq, double print_time
        , double pos_x, double pos_y, double pos_z);
    int trapq_extract_old(struct trapq *tq, struct pull_move *p, int max
        , double start_time, double end_time);
"""

defs_kin_cartesian = """
    struct stepper_kinematics *cartesian_stepper_alloc(char axis);
"""
defs_kin_generic_cartesian = """
    struct stepper_kinematics *generic_cartesian_stepper_alloc(double a_x
        , double a_y, double a_z);
    void generic_cartesian_stepper_set_coeffs(struct stepper_kinematics *sk
        , double a_x, double a_y, double a_z);
"""

defs_kin_corexy = """
    struct stepper_kinematics *corexy_stepper_alloc(char type);
"""

defs_kin_corexz = """
    struct stepper_kinematics *corexz_stepper_alloc(char type);
"""

defs_kin_delta = """
    struct stepper_kinematics *delta_stepper_alloc(double arm2
        , double tower_x, double tower_y);
"""

defs_kin_deltesian = """
    struct stepper_kinematics *deltesian_stepper_alloc(double arm2
        , double arm_x);
"""

defs_kin_polar = """
    struct stepper_kinematics *polar_stepper_alloc(char type);
"""

defs_kin_rotary_delta = """
    struct stepper_kinematics *rotary_delta_stepper_alloc(
        double shoulder_radius, double shoulder_height
        , double angle, double upper_arm, double lower_arm);
"""

defs_kin_winch = """
    struct stepper_kinematics *winch_stepper_alloc(double anchor_x
        , double anchor_y, double anchor_z);
"""

defs_kin_extruder = """
    struct stepper_kinematics *extruder_stepper_alloc(void);
    void extruder_stepper_free(struct stepper_kinematics *sk);
    void extruder_set_pressure_advance(struct stepper_kinematics *sk
        , double print_time, double pressure_advance, double smooth_time);
"""

defs_kin_shaper = """
    int input_shaper_set_shaper_params(struct stepper_kinematics *sk, char axis
        , int n, double a[], double t[]);
    int input_shaper_set_sk(struct stepper_kinematics *sk
        , struct stepper_kinematics *orig_sk);
    void input_shaper_update_sk(struct stepper_kinematics *sk);
    struct stepper_kinematics * input_shaper_alloc(void);
"""

defs_kin_idex = """
    void dual_carriage_set_sk(struct stepper_kinematics *sk
        , struct stepper_kinematics *orig_sk);
    int dual_carriage_set_transform(struct stepper_kinematics *sk
        , char axis, double scale, double offs);
    struct stepper_kinematics * dual_carriage_alloc(void);
"""

defs_serialqueue = """
    #define MESSAGE_MAX 64
    struct pull_queue_message {
        uint8_t msg[MESSAGE_MAX];
        int len;
        double sent_time, receive_time;
        uint64_t notify_id;
    };

    struct serialqueue *serialqueue_alloc(int serial_fd, char serial_fd_type
        , int client_id, char name[16]);
    void serialqueue_exit(struct serialqueue *sq);
    void serialqueue_free(struct serialqueue *sq);
    struct command_queue *serialqueue_alloc_commandqueue(void);
    void serialqueue_free_commandqueue(struct command_queue *cq);
    void serialqueue_send(struct serialqueue *sq, struct command_queue *cq
        , uint8_t *msg, int len, uint64_t min_clock, uint64_t req_clock
        , uint64_t notify_id);
    void serialqueue_pull(struct serialqueue *sq
        , struct pull_queue_message *pqm);
    void serialqueue_set_wire_frequency(struct serialqueue *sq
        , double frequency);
    void serialqueue_set_receive_window(struct serialqueue *sq
        , int receive_window);
    void serialqueue_set_clock_est(struct serialqueue *sq, double est_freq
        , double conv_time, uint64_t conv_clock, uint64_t last_clock);
    void serialqueue_get_stats(struct serialqueue *sq, char *buf, int len);
    int serialqueue_extract_old(struct serialqueue *sq, int sentq
        , struct pull_queue_message *q, int max);
"""

defs_trdispatch = """
    void trdispatch_start(struct trdispatch *td, uint32_t dispatch_reason);
    void trdispatch_stop(struct trdispatch *td);
    struct trdispatch *trdispatch_alloc(void);
    struct trdispatch_mcu *trdispatch_mcu_alloc(struct trdispatch *td
        , struct serialqueue *sq, struct command_queue *cq, uint32_t trsync_oid
        , uint32_t set_timeout_msgtag, uint32_t trigger_msgtag
        , uint32_t state_msgtag);
    void trdispatch_mcu_setup(struct trdispatch_mcu *tdm
        , uint64_t last_status_clock, uint64_t expire_clock
        , uint64_t expire_ticks, uint64_t min_extend_ticks);
"""

defs_pyhelper = """
    void set_python_logging_callback(void (*func)(const char *));
    double get_monotonic(void);
    int set_thread_name(char name[16]);
"""

defs_std = """
    void free(void*);
"""

defs_all = [
    defs_pyhelper, defs_serialqueue, defs_std, defs_stepcompress,
    defs_steppersync, defs_itersolve, defs_trapq, defs_trdispatch,
    defs_kin_cartesian, defs_kin_corexy, defs_kin_corexz, defs_kin_delta,
    defs_kin_deltesian, defs_kin_polar, defs_kin_rotary_delta, defs_kin_winch,
    defs_kin_extruder, defs_kin_shaper, defs_kin_idex,
    defs_kin_generic_cartesian,
]

# Update filenames to an absolute path
def get_abs_files(srcdir, filelist):
    return [os.path.join(srcdir, fname) for fname in filelist]

# Return the list of file modification times
def get_mtimes(filelist):
    out = []
    for filename in filelist:
        try:
            t = os.path.getmtime(filename)
        except os.error:
            continue
        out.append(t)
    return out

# Check if the code needs to be compiled
def check_build_code(sources, target):
    src_times = get_mtimes(sources)
    obj_times = get_mtimes([target])
    return not obj_times or max(src_times) > min(obj_times)

# Check if the current gcc version supports a particular command-line option
def check_gcc_option(option):
    cmd = "%s %s -S -o /dev/null -xc /dev/null > /dev/null 2>&1" % (
        GCC_CMD, option)
    res = os.system(cmd)
    return res == 0

# Check if the current gcc version supports a particular command-line option
def do_build_code(cmd):
    res = os.system(cmd)
    if res:
        msg = "Unable to build C code module (error=%s)" % (res,)
        logging.error(msg)
        raise Exception(msg)

# Build the main c_helper.so c code library
def check_build_c_library():
    srcdir = os.path.dirname(os.path.realpath(__file__))
    srcfiles = get_abs_files(srcdir, SOURCE_FILES)
    ofiles = get_abs_files(srcdir, OTHER_FILES)
    destlib = get_abs_files(srcdir, [DEST_LIB])[0]
    if not check_build_code(srcfiles+ofiles+[__file__], destlib):
        # Code already built
        return destlib
    # Select command line options
    if check_gcc_option(SSE_FLAGS):
        cmd = "%s %s %s" % (GCC_CMD, SSE_FLAGS, COMPILE_ARGS)
    else:
        cmd = "%s %s" % (GCC_CMD, COMPILE_ARGS)
    # Invoke compiler
    logging.info("Building C code module %s", DEST_LIB)
    tempdestlib = get_abs_files(srcdir, ["_temp_" + DEST_LIB])[0]
    do_build_code(cmd % (tempdestlib, ' '.join(srcfiles)))
    # Rename from temporary file to final file name
    os.rename(tempdestlib, destlib)
    return destlib

FFI_main = None
FFI_lib = None
pyhelper_logging_callback = None

# Helper invoked from C errorf() code to log errors
def logging_callback(msg):
    logging.error(FFI_main.string(msg))

# Return the Foreign Function Interface api to the caller
def get_ffi():
    global FFI_main, FFI_lib, pyhelper_logging_callback
    if FFI_lib is None:
        # Check if library needs to be built, and build if so
        destlib = check_build_c_library()
        # Open library
        FFI_main = cffi.FFI()
        for d in defs_all:
            FFI_main.cdef(d)
        FFI_lib = FFI_main.dlopen(destlib)
        # Setup error logging
        pyhelper_logging_callback = FFI_main.callback("void func(const char *)",
                                                      logging_callback)
        FFI_lib.set_python_logging_callback(pyhelper_logging_callback)
    return FFI_main, FFI_lib


######################################################################
# hub-ctrl hub power controller
######################################################################

HC_COMPILE_CMD = "gcc -Wall -g -O2 -o %s %s -lusb"
HC_SOURCE_FILES = ['hub-ctrl.c']
HC_SOURCE_DIR = '../../lib/hub-ctrl'
HC_TARGET = "hub-ctrl"
HC_CMD = "sudo %s/hub-ctrl -h 0 -P 2 -p %d"

def run_hub_ctrl(enable_power):
    srcdir = os.path.dirname(os.path.realpath(__file__))
    hubdir = os.path.join(srcdir, HC_SOURCE_DIR)
    srcfiles = get_abs_files(hubdir, HC_SOURCE_FILES)
    destlib = get_abs_files(hubdir, [HC_TARGET])[0]
    if check_build_code(srcfiles, destlib):
        logging.info("Building C code module %s", HC_TARGET)
        do_build_code(HC_COMPILE_CMD % (destlib, ' '.join(srcfiles)))
    os.system(HC_CMD % (hubdir, enable_power))


if __name__ == '__main__':
    get_ffi()
