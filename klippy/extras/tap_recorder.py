import json, logging, os


class TapRecorder:
    def __init__(self, config):
        self._is_recording = False
        self._tap_file_name = None
        self._printer = config.get_printer()
        self._probe_name = config.get('load_cell_probe_name',
            default='load_cell_probe')
        self._register_commands()
        self._printer.register_event_handler("klippy:connect", self._on_connect)

    def _on_connect(self):
        load_cell_probe = self._printer.lookup_object(self._probe_name)
        load_cell_probe.add_client(self._on_tap)

    def _on_tap(self, tap_event):
        if self._is_recording:
            self._save_tap(tap_event['tap'])
        return True

    def _save_tap(self, tap_data):
        data = []
        if (os.path.exists(self._tap_file_name)
                and os.path.getsize(self._tap_file_name) > 0):
            try:
                with open(self._tap_file_name, "r", encoding="utf-8") as f:
                    data = json.load(f)  # Load existing JSON data
                    if not isinstance(data, list):
                        raise ValueError("JSON file does not contain an array")
            except Exception as e:
                logging.error("Failed to read file contents", e)
                raise e
        data.append(tap_data)
        try:
            with open(self._tap_file_name, "w", encoding="utf-8") as f:
                f.write(json.dumps(data, indent=4))
        except Exception as e:
            logging.error("Failed to write file", e)
            raise e

    cmd_start_recording_help = "Start recording tap data"

    def cmd_start_recording(self, gcmd):
        full_path = "_"
        try:
            self._tap_file_name = gcmd.get("FILE", default=None)
            full_path = os.path.abspath(self._tap_file_name)
            gcmd.respond_info("Saving tap data to %s" % (full_path,))
            open(self._tap_file_name, "w", encoding="utf-8")
            self._is_recording = True
        except Exception as e:
            error_msg = "Failed to open file %s" % (full_path,)
            raise gcmd.error(error_msg)

    cmd_stop_recording_help = "Stop recording tap data"

    def cmd_stop_recording(self, gcmd):
        self._is_recording = False

    def _register_commands(self):
        # Register commands
        gcode = self._printer.lookup_object('gcode')
        gcode.register_command("TAP_RECORDER_START",
            self.cmd_start_recording,
            desc=self.cmd_start_recording_help)
        gcode.register_command("TAP_RECORDER_STOP",
            self.cmd_stop_recording,
            desc=self.cmd_stop_recording_help)


def load_config(config):
    return TapRecorder(config)
