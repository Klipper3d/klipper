# Basic LCD display support
#
# Copyright (C) 2018-2020  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2018  Aleph Objects, Inc <marcio@alephobjects.com>
# Copyright (C) 2018  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, os, ast
import hd44780, st7920, uc1701, menu

LCD_chips = {
    'st7920': st7920.ST7920, 'hd44780': hd44780.HD44780,
    'uc1701': uc1701.UC1701, 'ssd1306': uc1701.SSD1306, 'sh1106': uc1701.SH1106,
}

# Storage of [display_template my_template] config sections
class DisplayTemplate:
    def __init__(self, config):
        self.printer = config.get_printer()
        name_parts = config.get_name().split()
        if len(name_parts) != 2:
            raise config.error("Section name '%s' is not valid"
                               % (config.get_name(),))
        self.name = name_parts[1]
        self.params = {}
        for option in config.get_prefix_options('param_'):
            try:
                self.params[option] = ast.literal_eval(config.get(option))
            except ValueError as e:
                raise config.error(
                    "Option '%s' in section '%s' is not a valid literal" % (
                        option, config.get_name()))
        gcode_macro = self.printer.try_load_module(config, 'gcode_macro')
        self.template = gcode_macro.load_template(config, 'text')
    def render(self, context, **kwargs):
        params = dict(self.params)
        params.update(**kwargs)
        if len(params) != len(self.params):
            raise self.printer.command_error(
                "Invalid parameter to display_template %s" % (self.name,))
        context = dict(context)
        context.update(params)
        return self.template.render(context)

# Store [display_data my_group my_item] sections (one instance per group name)
class DisplayGroup:
    def __init__(self, config, name, data_configs):
        # Load and parse the position of display_data items
        items = []
        for c in data_configs:
            pos = c.get('position')
            try:
                row, col = [int(v.strip()) for v in pos.split(',')]
            except:
                raise config.error("Unable to parse 'position' in section '%s'"
                                   % (c.get_name(),))
            items.append((row, col, c.get_name()))
        # Load all templates and store sorted by display position
        configs_by_name = {c.get_name(): c for c in data_configs}
        printer = config.get_printer()
        gcode_macro = printer.try_load_module(config, 'gcode_macro')
        self.data_items = []
        for row, col, name in sorted(items):
            c = configs_by_name[name]
            if c.get('text'):
                template = gcode_macro.load_template(c, 'text')
                self.data_items.append((row, col, template))
    def show(self, display, templates, eventtime):
        swrap = self.data_items[0][2].create_status_wrapper(eventtime)
        context = { 'printer': swrap,
                    'draw_progress_bar': display.draw_progress_bar }
        def render(name, **kwargs):
            return templates[name].render(context, **kwargs)
        context['render'] = render
        for row, col, template in self.data_items:
            text = template.render(context)
            display.draw_text(row, col, text.replace('\n', ''), eventtime)

class PrinterLCD:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        # Load low-level lcd handler
        self.lcd_chip = config.getchoice('lcd_type', LCD_chips)(config)
        self.lcd_type = config.get('lcd_type')
        # Load menu and display_status
        self.menu = None
        name = config.get_name()
        if name == 'display':
            # only load menu for primary display
            self.menu = menu.MenuManager(config, self.lcd_chip)
        self.printer.try_load_module(config, "display_status")
        # Configurable display
        self.display_templates = {}
        self.display_data_groups = {}
        self.load_config(config)
        dgroup = "_default_16x4"
        if self.lcd_chip.get_dimensions()[0] == 20:
            dgroup = "_default_20x4"
        dgroup = config.get('display_group', dgroup)
        self.show_data_group = self.display_data_groups.get(dgroup)
        if self.show_data_group is None:
            raise config.error("Unknown display_data group '%s'"
                               % (dgroup,))
        # Screen updating
        self.glyph_helpers = { 'animated_bed': self.animate_bed,
                               'animated_fan': self.animate_fan }
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.screen_update_timer = self.reactor.register_timer(
            self.screen_update_event)
    # Configurable display
    def load_config(self, config):
        # Load default display config file
        pconfig = self.printer.lookup_object('configfile')
        filename = os.path.join(os.path.dirname(__file__), 'display.cfg')
        try:
            dconfig = pconfig.read_config(filename)
        except Exception:
            raise self.printer.config_error("Cannot load config '%s'"
                                            % (filename,))
        # Load display_template sections
        dt_main = config.get_prefix_sections('display_template ')
        dt_main_names = { c.get_name(): 1 for c in dt_main }
        dt_def = [c for c in dconfig.get_prefix_sections('display_template ')
                  if c.get_name() not in dt_main_names]
        for c in dt_main + dt_def:
            dt = DisplayTemplate(c)
            self.display_templates[dt.name] = dt
        # Load display_data sections
        dd_main = config.get_prefix_sections('display_data ')
        dd_main_names = { c.get_name(): 1 for c in dd_main }
        dd_def = [c for c in dconfig.get_prefix_sections('display_data ')
                  if c.get_name() not in dd_main_names]
        groups = {}
        for c in dd_main + dd_def:
            name_parts = c.get_name().split()
            if len(name_parts) != 3:
                raise config.error("Section name '%s' is not valid"
                                   % (c.get_name(),))
            groups.setdefault(name_parts[1], []).append(c)
        for group_name, data_configs in groups.items():
            dg = DisplayGroup(config, group_name, data_configs)
            self.display_data_groups[group_name] = dg
    # Initialization
    def handle_ready(self):
        self.lcd_chip.init()
        # Start screen update timer
        self.reactor.update_timer(self.screen_update_timer, self.reactor.NOW)
    # Screen updating
    def screen_update_event(self, eventtime):
        # update menu component
        if self.menu is not None:
            ret = self.menu.screen_update_event(eventtime)
            if ret:
                return ret
        # Update normal display
        self.lcd_chip.clear()
        try:
            self.show_data_group.show(self, self.display_templates, eventtime)
        except:
            logging.exception("Error during display screen update")
        self.lcd_chip.flush()
        return eventtime + .500
    # Rendering helpers
    def animate_bed(self, row, col, eventtime):
        frame = int(eventtime) & 1
        return self.lcd_chip.write_glyph(col, row, 'bed_heat%d' % (frame + 1,))
    def animate_fan(self, row, col, eventtime):
        frame = int(eventtime) & 1
        return self.lcd_chip.write_glyph(col, row, 'fan%d' % (frame + 1,))
    def draw_text(self, row, col, mixed_text, eventtime):
        pos = col
        for i, text in enumerate(mixed_text.split('~')):
            if i & 1 == 0:
                # write text
                self.lcd_chip.write_text(pos, row, text)
                pos += len(text)
            elif text in self.glyph_helpers:
                pos += self.glyph_helpers[text](row, pos, eventtime)
            else:
                # write glyph
                pos += self.lcd_chip.write_glyph(pos, row, text)
    def draw_progress_bar(self, row, col, width, value):
        value = int(value * 100.)
        data = [0x00] * width
        char_pcnt = int(100/width)
        for i in range(width):
            if (i+1)*char_pcnt <= value:
                # Draw completely filled bytes
                data[i] |= 0xFF
            elif (i*char_pcnt) < value:
                # Draw partially filled bytes
                data[i] |= (-1 << 8-((value % char_pcnt)*8/char_pcnt)) & 0xff
        data[0] |= 0x80
        data[-1] |= 0x01
        self.lcd_chip.write_graphics(col, row, 0, [0xff]*width)
        for i in range(1, 15):
            self.lcd_chip.write_graphics(col, row, i, data)
        self.lcd_chip.write_graphics(col, row, 15, [0xff]*width)
        return ""

def load_config(config):
    return PrinterLCD(config)
