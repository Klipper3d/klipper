# Wrapper around C helper code
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, logging
import cffi

COMPILE_CMD = "gcc -Wall -g -O -shared -fPIC -o %s %s"
SOURCE_FILES = ['stepcompress.c', 'serialqueue.c', 'pyhelper.c']
DEST_LIB = "c_helper.so"
OTHER_FILES = ['list.h', 'serialqueue.h', 'pyhelper.h']

defs_stepcompress = """
    struct stepcompress *stepcompress_alloc(uint32_t max_error
        , uint32_t queue_step_msgid, uint32_t set_next_step_dir_msgid
        , uint32_t invert_sdir, uint32_t oid);
    void stepcompress_free(struct stepcompress *sc);
    int stepcompress_push(struct stepcompress *sc, double step_clock
        , int32_t sdir);
    int32_t stepcompress_push_factor(struct stepcompress *sc
        , double steps, double step_offset
        , double clock_offset, double factor);
    int32_t stepcompress_push_sqrt(struct stepcompress *sc
        , double steps, double step_offset
        , double clock_offset, double sqrt_offset, double factor);
    int32_t stepcompress_push_delta_const(struct stepcompress *sc
        , double clock_offset, double dist, double start_pos
        , double inv_velocity, double step_dist, double height
        , double closestxy_d, double closest_height2, double movez_r);
    int32_t stepcompress_push_delta_accel(struct stepcompress *sc
        , double clock_offset, double dist, double start_pos
        , double accel_multiplier, double step_dist, double height
        , double closestxy_d, double closest_height2, double movez_r);
    int stepcompress_reset(struct stepcompress *sc, uint64_t last_step_clock);
    int stepcompress_set_homing(struct stepcompress *sc, uint64_t homing_clock);
    int stepcompress_queue_msg(struct stepcompress *sc, uint32_t *data, int len);

    struct steppersync *steppersync_alloc(struct serialqueue *sq
        , struct stepcompress **sc_list, int sc_num, int move_num);
    void steppersync_free(struct steppersync *ss);
    int steppersync_flush(struct steppersync *ss, uint64_t move_clock);
"""

defs_serialqueue = """
    #define MESSAGE_MAX 64
    struct pull_queue_message {
        uint8_t msg[MESSAGE_MAX];
        int len;
        double sent_time, receive_time;
    };

    struct serialqueue *serialqueue_alloc(int serial_fd, int write_only);
    void serialqueue_exit(struct serialqueue *sq);
    void serialqueue_free(struct serialqueue *sq);
    struct command_queue *serialqueue_alloc_commandqueue(void);
    void serialqueue_free_commandqueue(struct command_queue *cq);
    void serialqueue_send(struct serialqueue *sq, struct command_queue *cq
        , uint8_t *msg, int len, uint64_t min_clock, uint64_t req_clock);
    void serialqueue_encode_and_send(struct serialqueue *sq
        , struct command_queue *cq, uint32_t *data, int len
        , uint64_t min_clock, uint64_t req_clock);
    void serialqueue_pull(struct serialqueue *sq, struct pull_queue_message *pqm);
    void serialqueue_set_baud_adjust(struct serialqueue *sq, double baud_adjust);
    void serialqueue_set_clock_est(struct serialqueue *sq, double est_clock
        , double last_ack_time, uint64_t last_ack_clock);
    void serialqueue_get_stats(struct serialqueue *sq, char *buf, int len);
    int serialqueue_extract_old(struct serialqueue *sq, int sentq
        , struct pull_queue_message *q, int max);
"""

defs_pyhelper = """
    void set_python_logging_callback(void (*func)(const char *));
"""

# Return the list of file modification times
def get_mtimes(srcdir, filelist):
    out = []
    for filename in filelist:
        pathname = os.path.join(srcdir, filename)
        try:
            t = os.path.getmtime(pathname)
        except os.error:
            continue
        out.append(t)
    return out

# Check if the code needs to be compiled
def check_build_code(srcdir):
    src_times = get_mtimes(srcdir, SOURCE_FILES + OTHER_FILES)
    obj_times = get_mtimes(srcdir, [DEST_LIB])
    if not obj_times or max(src_times) > min(obj_times):
        logging.info("Building C code module")
        srcfiles = [os.path.join(srcdir, fname) for fname in SOURCE_FILES]
        destlib = os.path.join(srcdir, DEST_LIB)
        os.system(COMPILE_CMD % (destlib, ' '.join(srcfiles)))

FFI_main = None
FFI_lib = None
pyhelper_logging_callback = None

# Return the Foreign Function Interface api to the caller
def get_ffi():
    global FFI_main, FFI_lib, pyhelper_logging_callback
    if FFI_lib is None:
        srcdir = os.path.dirname(os.path.realpath(__file__))
        check_build_code(srcdir)
        FFI_main = cffi.FFI()
        FFI_main.cdef(defs_stepcompress)
        FFI_main.cdef(defs_serialqueue)
        FFI_main.cdef(defs_pyhelper)
        FFI_lib = FFI_main.dlopen(os.path.join(srcdir, DEST_LIB))
        # Setup error logging
        def logging_callback(msg):
            logging.error(FFI_main.string(msg))
        pyhelper_logging_callback = FFI_main.callback(
            "void(const char *)", logging_callback)
        FFI_lib.set_python_logging_callback(pyhelper_logging_callback)
    return FFI_main, FFI_lib
