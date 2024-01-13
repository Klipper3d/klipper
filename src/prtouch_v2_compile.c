#include <math.h>
#include <stddef.h> // size_t
#include <stdint.h> // uint8_t
#include <stdlib.h>
#include <string.h> // memset

#include "board/gpio.h"     // struct gpio_in
#include "board/irq.h"      // irq_disable
#include "board/misc.h"     // timer_from_us
#include "board/internal.h" // gpio_peripheral
#include "command.h"        // DECL_COMMAND
#include "sched.h"          // struct timer
#include "stepper.h"


DECL_COMMAND(command_config_step_prtouch, "config_step_prtouch oid=%c step_cnt=%c swap_pin=%u sys_time_duty=%u");
DECL_COMMAND(command_add_step_prtouch, "add_step_prtouch oid=%c index=%c dir_pin=%u step_pin=%u dir_invert=%c step_invert=%c");
DECL_COMMAND(command_read_swap_prtouch, "read_swap_prtouch oid=%c");
DECL_COMMAND(command_start_step_prtouch, "start_step_prtouch oid=%c dir=%c send_ms=%c step_cnt=%u step_us=%u acc_ctl_cnt=%u low_spd_nul=%c send_step_duty=%c auto_rtn=%c");
DECL_COMMAND(command_manual_get_steps, "manual_get_steps oid=%c index=%c");
DECL_COMMAND(command_config_pres_prtouch, "config_pres_prtouch oid=%c use_adc=%c pres_cnt=%c swap_pin=%u sys_time_duty=%u");
DECL_COMMAND(command_add_pres_prtouch, "add_pres_prtouch oid=%c index=%c clk_pin=%u sda_pin=%u");
DECL_COMMAND(command_write_swap_prtouch, "write_swap_prtouch oid=%c sta=%c");
DECL_COMMAND(command_read_pres_prtouch, "read_pres_prtouch oid=%c acq_ms=%u cnt=%u");
DECL_COMMAND(command_deal_avgs_prtouch, "deal_avgs_prtouch oid=%c base_cnt=%c");
DECL_COMMAND(command_start_pres_prtouch, "start_pres_prtouch oid=%c tri_dir=%c acq_ms=%c send_ms=%c need_cnt=%c tri_hftr_cut=%u tri_lftr_k1=%u min_hold=%u max_hold=%u");
DECL_COMMAND(command_manual_get_pres, "manual_get_pres oid=%c index=%c");

void
sendf_info(void)
{
	uint32_t args[6] = {0};

	uint32_t ver = 0;

	uint8_t oid = 0,sta = 0;

	uint32_t i = 0,time = 0,buf[8] = {0};

	int32_t vals[4] = {0}; 

	uint8_t chs = 0;
	
	uint32_t cnt = 0;

	sendf("debug_prtouch oid=%c version=%u arg[0]=%u arg[1]=%u arg[2]=%u arg[3]=%u arg[4]=%u arg[5]=%u",
          (uint8_t)args[0], ver, (uint32_t)args[0], (uint32_t)args[1], (uint32_t)args[2], (uint32_t)args[3], (uint32_t)args[4], (uint32_t)args[5]);

	sendf("result_read_swap_prtouch oid=%c sta=%c", oid, sta);

	sendf("result_manual_get_steps oid=%c index=%c tri_time=%u tick0=%u tick1=%u tick2=%u tick3=%u step0=%u step1=%u step2=%u step3=%u",
        oid, i, time,
        buf[i + 0], buf[i + 1], buf[i + 2], buf[i + 3],
        buf[i + 0], buf[i + 1], buf[i + 2], buf[i + 3]);

	sendf("result_run_step_prtouch oid=%c index=%c tri_time=%u tick0=%u tick1=%u tick2=%u tick3=%u step0=%u step1=%u step2=%u step3=%u",
        (uint8_t)oid, i, time,
        buf[i + 0], buf[i + 1], buf[i + 2], buf[i + 3],
        buf[i + 0], buf[i + 1], buf[i + 2], buf[i + 3]);

	sendf("resault_write_swap_prtouch oid=%c", oid);

	sendf("result_deal_avgs_prtouch oid=%c ch0=%i ch1=%i ch2=%i ch3=%i", oid, vals[0], vals[1], vals[2], vals[3]);

	sendf("resault_manual_get_pres oid=%c index=%c tri_time=%u tri_chs=%c buf_cnt=%u tick_0=%u ch0_0=%i ch1_0=%i ch2_0=%i ch3_0=%i tick_1=%u ch0_1=%i ch1_1=%i ch2_1=%i ch3_1=%i",
        (uint8_t)oid, i, time, chs, cnt,
        buf[i + 0], buf[i + 0], buf[i + 0], buf[i + 0], buf[i + 0],
        buf[i + 1], buf[i + 1], buf[i + 1], buf[i + 1], buf[i + 1]);

	sendf("result_read_pres_prtouch oid=%c tick=%u ch0=%i ch1=%i ch2=%i ch3=%i", oid, time, vals[0], vals[1], vals[2], vals[3]);

	sendf("result_run_pres_prtouch oid=%c index=%c tri_time=%u tri_chs=%c buf_cnt=%u tick_0=%u ch0_0=%i ch1_0=%i ch2_0=%i ch3_0=%i tick_1=%u ch0_1=%i ch1_1=%i ch2_1=%i ch3_1=%i",
        (uint8_t)oid, i, time, chs, cnt,
        buf[i + 0], buf[i + 0], buf[i + 0], buf[i + 0], buf[i + 0],
        buf[i + 1], buf[i + 1], buf[i + 1], buf[i + 1], buf[i + 1]);
}
