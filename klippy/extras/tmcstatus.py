import logging

TRINAMIC_DRIVERS = ["tmc2130", "tmc2208", "tmc2209", "tmc2240", "tmc2660", "tmc5160"]
STEPPERS = ['stepper_x', 'stepper_y', 'stepper_z', 'extruder']
RSENSES = {
    "tmc2209": 0.11
}

class TMCStatus:
    def __init__(self, config):
        self.config = config
        self.printer = config.get_printer()
        self.configured_steppers = []
        self.tmcs = {}

        for driver in TRINAMIC_DRIVERS:
            self.configured_steppers.extend([n.get_name() for n in self.config.get_prefix_sections(driver)])
        
        self.handle_connect()

    def handle_connect(self):
        for s in self.configured_steppers:
            tmc = self.printer.lookup_object(s)
            self.tmcs[s] = tmc

    def get_status(self, eventtime):
        data = {}
        for tmc, tmcobj in self.tmcs.items():

            drv_status_val = tmcobj.mcu_tmc.get_register('DRV_STATUS')
            fields = tmcobj.fields.get_reg_fields('DRV_STATUS', drv_status_val)
            drv_fields = {n: v for n, v in fields.items() if v}
            tmc_data = {
                'drv_status': drv_fields,
                
                'hstrt': tmcobj.fields.get_field('hstrt'),
                'hend': tmcobj.fields.get_field('hend'),
                
                'pwm_autoscale': tmcobj.fields.get_field('pwm_autoscale'),
                'pwm_autograd': tmcobj.fields.get_field('pwm_autograd'),
                'pwm_grad': tmcobj.fields.get_field('pwm_grad'),
                'pwm_ofs': tmcobj.fields.get_field('pwm_ofs'),
                'pwm_reg': tmcobj.fields.get_field('pwm_reg'),
                'pwm_lim': tmcobj.fields.get_field('pwm_lim'),
                'tpwmthrs': tmcobj.fields.get_field('tpwmthrs'),

                'en_spreadcycle': tmcobj.fields.get_field('en_spreadcycle'),
                'tbl': tmcobj.fields.get_field('tbl'),
                'toff': tmcobj.fields.get_field('toff'),

                'tcoolthrs': tmcobj.fields.get_field('tcoolthrs'),
                
                'sg_result': tmcobj.mcu_tmc.get_register('SG_RESULT'),
                'semin': tmcobj.fields.get_field('semin'),
                'semax': tmcobj.fields.get_field('semax'),
                'seup': tmcobj.fields.get_field('seup'),
                'sedn': tmcobj.fields.get_field('sedn'),
                'seimin': tmcobj.fields.get_field('seimin')
            }
            if 'cs_actual' in drv_fields:
                tmc_data['i_rms'] = self._cs_to_rms(drv_fields['cs_actual'], tmc.split()[0], tmcobj)

            if tmcobj.fields.lookup_register('en_pwm_mode', None):
                tmc_data['en_pwm_mode'] = tmcobj.fields.get_field('en_pwm_mode')

            data[tmc] = tmc_data

        return data

    def _cs_to_rms(self, cs, tmc, tmcobj):
        rsense = RSENSES.get(tmc)
        vsense = tmcobj.fields.get_field('vsense')
        return (cs+1)/32.0 * (0.180 if vsense == 1 else 0.325)/(rsense+0.02) / 1.41421 * 1000;

def load_config(config):
    return TMCStatus(config)
