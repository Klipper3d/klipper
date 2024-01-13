# Pause/Resume functionality with position capture/restore
#
# Copyright (C) 2019  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, json, logging
from .tool import reportInformation

class PauseResume:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object('gcode')
        self.recover_velocity = config.getfloat('recover_velocity', 50.)
        self.v_sd = None
        self.is_paused = False
        self.sd_paused = False
        self.pause_command_sent = False
        self.config = config
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.gcode.register_command("PAUSE", self.cmd_PAUSE,
                                    desc=self.cmd_PAUSE_help)
        self.gcode.register_command("RESUME", self.cmd_RESUME,
                                    desc=self.cmd_RESUME_help)
        self.gcode.register_command("CLEAR_PAUSE", self.cmd_CLEAR_PAUSE,
                                    desc=self.cmd_CLEAR_PAUSE_help)
        self.gcode.register_command("CANCEL_PRINT", self.cmd_CANCEL_PRINT,
                                    desc=self.cmd_CANCEL_PRINT_help)
        webhooks = self.printer.lookup_object('webhooks')
        webhooks.register_endpoint("pause_resume/cancel_continue_print",
                                   self._handle_cancel_continue_print_request)
        webhooks.register_endpoint("pause_resume/check_continue_print_state",
                                   self._check_power_loss_state_request)
        webhooks.register_endpoint("pause_resume/set_print_first_layer",
                                   self._set_print_first_layer_request)
        webhooks.register_endpoint("pause_resume/cancel",
                                   self._handle_cancel_request)
        webhooks.register_endpoint("pause_resume/pause",
                                   self._handle_pause_request)
        webhooks.register_endpoint("pause_resume/resume",
                                   self._handle_resume_request)
        webhooks.register_endpoint("getBootLoaderVersion",
                                   self._getBootLoaderVersion)
        self._setBootLoaderStateCmdOid = None
    def handle_connect(self):
        self.v_sd = self.printer.lookup_object('virtual_sdcard', None)
        
    def _getBootLoaderVersion(self, web_request):
        mcu = self.printer.lookup_object('mcu')
        result = mcu.get_constants().get('software_version', '')
        web_request.send({'software_version': result})
        return {"software_version": result}
    
    def _setBootLoaderState(self, web_request):
        mcu = self.printer.lookup_object('mcu')
        oid = mcu.create_oid() if not self._setBootLoaderStateCmdOid else self._setBootLoaderStateCmdOid
        self._setBootLoaderStateCmdOid = oid
        mcu.add_config_cmd("config_usrboot oid=%d" % (oid,))
        # sendf("usrboot_ack oid=%c enter_boot_status=%c",args[0],status)
        result = mcu.lookup_query_command("jump_to_usrboot_query oid=%c", "usrboot_ack oid=%c enter_boot_status=%c", oid=oid).send()
        return {"result": result}

    def _set_print_first_layer_request(self, web_request): 
        self.v_sd.first_layer_stop = False
        self.v_sd.print_first_layer = False
        response = {"state": "success"}
        web_request.send(response)
        return response

    def _check_power_loss_state_request(self, web_request): 
        from subprocess import call
        response = {"file_state": False, "eeprom_state": False}
        if os.path.exists(self.v_sd.print_file_name_path):
            try:
                with open(self.v_sd.print_file_name_path, "r") as f:
                    data = f.read()
                    if len(data) == 0:
                        logging.error("%s f.read()==None read fail!!!" % self.v_sd.print_file_name_path)
                    response["file_state"] = True if json.loads(data) else False
            except Exception as err:
                os.remove(self.v_sd.print_file_name_path)
                bl24c16f = self.printer.lookup_object('bl24c16f') if "bl24c16f" in self.printer.objects else None
                if bl24c16f:
                    self.gcode.run_script("EEPROM_WRITE_BYTE ADDR=1 VAL=255")
                logging.exception(err)
        power_loss_switch = False
        if os.path.exists(self.v_sd.user_print_refer_path):
            with open(self.v_sd.user_print_refer_path, "r") as f:
                data = json.loads(f.read())
                power_loss_switch = data.get("power_loss", {}).get("switch", False)
        bl24c16f = self.printer.lookup_object('bl24c16f') if "bl24c16f" in self.printer.objects else None
        eepromState = bl24c16f.checkEepromFirstEnable() if power_loss_switch and bl24c16f else True
        if not eepromState:
            response["eeprom_state"] = True
        print_stats = self.printer.lookup_object('print_stats', None)
        if response["file_state"] == True and response["eeprom_state"] == True and print_stats and print_stats.state == "standby":
            print_stats.power_loss = 1
        if print_stats and print_stats.state != "standby":
            response["file_state"] = False
            response["eeprom_state"] = False
            logging.info("current printer state:%s" % print_stats.state)
        if os.path.exists(self.gcode.exclude_object_info) and (response["file_state"]==False or response["eeprom_state"]==False):
            os.remove(self.gcode.exclude_object_info)
        web_request.send(response)
        return response
    
    def _handle_cancel_continue_print_request(self, web_request):
        from subprocess import call
        if os.path.exists(self.v_sd.print_file_name_path):
            os.remove(self.v_sd.print_file_name_path)
        if os.path.exists(self.gcode.exclude_object_info):
            os.remove(self.gcode.exclude_object_info)
        call("sync", shell=True)
        bl24c16f = self.printer.lookup_object('bl24c16f') if "bl24c16f" in self.printer.objects else None
        power_loss_switch = False
        if os.path.exists(self.v_sd.user_print_refer_path):
            with open(self.v_sd.user_print_refer_path, "r") as f:
                data = json.loads(f.read())
                power_loss_switch = data.get("power_loss", {}).get("switch", False)
        bl24c16f = self.printer.lookup_object('bl24c16f') if "bl24c16f" in self.printer.objects else None
        if power_loss_switch and bl24c16f:
            self.gcode.run_script("EEPROM_WRITE_BYTE ADDR=1 VAL=255")
            self.gcode.respond_info("cancel_continue_print:success")
        print_stats = self.printer.lookup_object('print_stats', None)
        if print_stats:
            print_stats.power_loss = 0
    def _handle_cancel_request(self, web_request):
        self.gcode.run_script("CANCEL_PRINT")
    def _handle_pause_request(self, web_request):
        self.gcode.run_script("PAUSE")
    def _handle_resume_request(self, web_request):
        self.gcode.run_script("RESUME")
    def get_status(self, eventtime):
        return {
            'is_paused': self.is_paused
        }
    def is_sd_active(self):
        return self.v_sd is not None and self.v_sd.is_active()
    def send_pause_command(self):
        # This sends the appropriate pause command from an event.  Note
        # the difference between pause_command_sent and is_paused, the
        # module isn't officially paused until the PAUSE gcode executes.
        if not self.pause_command_sent:
            if self.is_sd_active():
                # Printing from virtual sd, run pause command
                self.sd_paused = True
                self.v_sd.do_pause()
            else:
                self.sd_paused = False
                self.gcode.respond_info("action:paused")
            self.pause_command_sent = True
    cmd_PAUSE_help = ("Pauses the current print")
    def cmd_PAUSE(self, gcmd):
        import time
        reactor = self.printer.get_reactor()
        while self.v_sd.toolhead_moved:
            time.sleep(0.001)
            reactor.pause(reactor.monotonic() + .01)
        if self.is_paused:
            gcmd.respond_info("""{"code":"key211", "msg": "Print already paused", "values": []}""")
            return
        self.send_pause_command()
        self.gcode.run_script_from_command("SAVE_GCODE_STATE NAME=PAUSE_STATE")
        self.is_paused = True
        reportInformation("key601")
    def send_resume_command(self):
        if self.sd_paused:
            # Printing from virtual sd, run pause command
            self.v_sd.do_resume_status = True
            self.v_sd.do_resume()
            self.sd_paused = False
        else:
            self.gcode.respond_info("action:resumed")
        self.pause_command_sent = False
    cmd_RESUME_help = ("Resumes the print from a pause")
    def cmd_RESUME(self, gcmd):
        if not self.is_paused:
            gcmd.respond_info("""{"code": "key16", "msg": "Print is not paused, resume aborted"}""")
            return
        velocity = gcmd.get_float('VELOCITY', self.recover_velocity)
        self.gcode.run_script_from_command(
            "RESTORE_GCODE_STATE NAME=PAUSE_STATE MOVE=1 MOVE_SPEED=%.4f"
            % (velocity))
        self.send_resume_command()
        self.is_paused = False
        result = {}
        if os.path.exists(self.v_sd.print_file_name_path):
            with open(self.v_sd.print_file_name_path, "r") as f:
                result = (json.loads(f.read()))
                result["variable_z_safe_pause"] = 0
            with open(self.v_sd.print_file_name_path, "w") as f:
                f.write(json.dumps(result))
                f.flush()
        reportInformation("key602")
    cmd_CLEAR_PAUSE_help = (
        "Clears the current paused state without resuming the print")
    def cmd_CLEAR_PAUSE(self, gcmd):
        self.is_paused = self.pause_command_sent = False
    cmd_CANCEL_PRINT_help = ("Cancel the current print")
    def cmd_CANCEL_PRINT(self, gcmd):
        if self.is_sd_active() or self.sd_paused:
            self.v_sd.do_cancel()
        else:
            gcmd.respond_info("action:cancel")
        self.cmd_CLEAR_PAUSE(gcmd)
        reportInformation("key603")

def load_config(config):
    return PauseResume(config)
