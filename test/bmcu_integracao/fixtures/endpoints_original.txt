# SPDX-License-Identifier: GPL-3.0-or-later
class Endpoint:
    def _run_generic_contract_macro(self, key, label, material='', reason='load'):
        command = self._required_generic_macro(key, label)
        script = '%s ENDPOINT=%s MATERIAL=%s REASON=%s' % (
            command, self._gcode_param(self.name),
            self._gcode_param(material), self._gcode_param(reason))
        self._run(script, printer_priority=True)
