# Add 'RESPOND' and 'M118' commands for sending messages to the host
#
# Copyright (C) 2018  Alec Plumb <alec@etherwalker.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

respond_types = {
    'echo': 'echo:',
    'command': '//',
    'error' : '!!',
}

respond_types_no_space = {
    'echo_no_space': 'echo:',
}

class HostResponder:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.default_prefix = config.getchoice('default_type', respond_types,
                                               'echo')
        self.default_prefix = config.get('default_prefix', self.default_prefix)
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command('M118', self.cmd_M118, True)
        gcode.register_command('RESPOND', self.cmd_RESPOND, True,
                               desc=self.cmd_RESPOND_help)
    def cmd_M118(self, gcmd):
        msg = gcmd.get_raw_command_parameters()
        gcmd.respond_raw("%s %s" % (self.default_prefix, msg))
    cmd_RESPOND_help = ("Echo the message prepended with a prefix")
    def cmd_RESPOND(self, gcmd):
        no_space = False
        respond_type = gcmd.get('TYPE', None)
        prefix = self.default_prefix
        if(respond_type != None):
            respond_type = respond_type.lower()
            if(respond_type in respond_types):
                prefix = respond_types[respond_type]
            elif(respond_type in respond_types_no_space):
                prefix = respond_types_no_space[respond_type]
                no_space = True
            else:
                raise gcmd.error(
                    "RESPOND TYPE '%s' is invalid. Must be one"
                    " of 'echo', 'command', or 'error'" % (respond_type,))
        prefix = gcmd.get('PREFIX', prefix)
        msg = gcmd.get('MSG', '')
        if(no_space):
            gcmd.respond_raw("%s%s" % (prefix, msg))
        else:
            gcmd.respond_raw("%s %s" % (prefix, msg))

def load_config(config):
    return HostResponder(config)
