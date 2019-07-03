# -*- coding: utf-8 -*-
# Basic LCD menu support
#
# Based on the RaspberryPiLcdMenu from Alan Aufderheide, February 2013
# Copyright (C) 2018  Janar Sööt <janar.soot@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, logging


class error(Exception):
    pass


class sentinel:
    pass


# static class for cursor
class MenuCursor:
    NONE = ' '
    SELECT = '>'
    EDIT = '*'


# static class for type cast
class MenuHelper:
    @staticmethod
    def asliteral(s):
        s = str(s)
        if (s.startswith('"') and s.endswith('"')) or \
                (s.startswith("'") and s.endswith("'")):
            s = s[1:-1]
        return s

    @staticmethod
    def aslatin(s):
        if isinstance(s, str):
            return s
        elif isinstance(s, unicode):
            return unicode(s).encode('latin-1', 'ignore')
        else:
            return str(s)

    @staticmethod
    def asflatline(s):
        return ''.join(MenuHelper.aslatin(s).splitlines())

    @staticmethod
    def asbool(s, default=False):
        if s is None:
            return bool(default)
        elif isinstance(s, (bool, int, float)):
            return bool(s)
        elif MenuHelper.isfloat(s):
            return bool(MenuHelper.asfloat(s))
        s = str(s).strip()
        return s.lower() in ('y', 'yes', 't', 'true', 'on', '1')

    @staticmethod
    def asint(s, default=0):
        if s is None:
            return default
        if isinstance(s, (int, float)):
            return int(s)
        s = str(s).strip()
        return int(float(s)) if MenuHelper.isfloat(s) else int(default)

    @staticmethod
    def asfloat(s, default=0.0):
        if s is None:
            return default
        if isinstance(s, (int, float)):
            return float(s)
        s = str(s).strip()
        return float(s) if MenuHelper.isfloat(s) else float(default)

    @staticmethod
    def isfloat(value):
        try:
            float(value)
            return True
        except ValueError:
            return False

    @staticmethod
    def lines_aslist(value, default=[]):
        if isinstance(value, str):
            value = filter(None, [x.strip() for x in value.splitlines()])
        try:
            return list(value)
        except Exception:
            logging.exception("Lines as list parsing error")
            return list(default)

    @staticmethod
    def words_aslist(value, sep=',', default=[]):
        if isinstance(value, str):
            value = filter(None, [x.strip() for x in value.split(sep)])
        try:
            return list(value)
        except Exception:
            logging.exception("Words as list parsing error")
            return list(default)

    @staticmethod
    def aslist(value, flatten=True, default=[]):
        values = MenuHelper.lines_aslist(value)
        if not flatten:
            return values
        result = []
        for value in values:
            subvalues = MenuHelper.words_aslist(value, sep=',')
            result.extend(subvalues)
        return result

    @staticmethod
    def aschoice(config, option, choices, default=sentinel):
        c = config.get(option, default)
        if c not in choices:
            raise error("Choice '%s' for option '%s'"
                        " is not a valid choice" % (c, option))
        return choices[c]

    @staticmethod
    def seconds2(key):
        """Convert seconds to minutes, hours, days"""
        time = {}

        def time_fn(value):
            try:
                seconds = int(abs(value))
            except Exception:
                logging.exception("Seconds parsing error")
                seconds = 0
            time['days'], time['seconds'] = divmod(seconds, 86400)
            time['hours'], time['seconds'] = divmod(time['seconds'], 3600)
            time['minutes'], time['seconds'] = divmod(time['seconds'], 60)
            if key in time:
                return time[key]
            else:
                return 0
        return time_fn

    @staticmethod
    def interpolate(value, from_min, from_max, to_min, to_max):
        """Linear Interpolation, re-maps a number from one range to another"""
        from_span = from_max - from_min
        to_span = to_max - to_min
        scale_factor = float(to_span) / float(from_span)
        return to_min + (value - from_min) * scale_factor

    @staticmethod
    def sequence(eventtime):
        """Sequence animation"""
        def sequence_fn(frames, animate=True, offset=0):
            if isinstance(frames, (list, tuple)):
                offset = min(len(frames)-1, max(0, offset))
                frame = (offset+((int(eventtime) % len(frames[offset:])))) \
                    if animate else 0
                return frames[frame]
            else:
                return str(frames)
        return sequence_fn


# Menu item baseclass
class MenuItem(object):
    def __init__(self, manager, config):
        self.cursor = config.get('cursor', MenuCursor.SELECT)
        self._manager = manager
        self._width = MenuHelper.asint(config.get('width', '0'))
        self._scroll = MenuHelper.asbool(config.get('scroll', 'false'))
        self._enable_tpl = manager.gcode_macro.load_template(
            config, 'enable', 'True')
        self._name_tpl = manager.gcode_macro.load_template(
            config, 'name')
        self._last_heartbeat = None
        self.__scroll_offs = 0
        self.__scroll_diff = 0
        self.__scroll_dir = None
        self.__last_state = True
        if len(self.cursor) < 1:
            raise error("Cursor with unexpected length, expecting 1.")
        # id: item id - used when sending events
        # ns: item namespace - used in item relative paths
        if isinstance(config, dict):
            self._id = config.get('__id', hex(id(self)))
            self._ns = config.get('__ns', '')
        else:
            self._id = self._ns = " ".join(config.get_name().split()[1:])
        # if scroll is enabled and width is not specified then
        # display width is used and adjusted by cursor size
        if self._scroll and not self._width:
            self._width = self.manager.cols - len(self.cursor)
        self.init()

    # override
    def init(self):
        pass

    def _name(self):
        context = self.get_context()
        return MenuHelper.asliteral(MenuHelper.asflatline(
            self._name_tpl.render(context)))

    # override
    def _render(self):
        return self._name()

    # override
    def _second_tick(self, eventtime):
        pass

    # override
    def is_editing(self):
        return False

    # override
    def is_readonly(self):
        return True

    # override
    def is_scrollable(self):
        return True

    # override
    def is_enabled(self):
        return self.eval_enable()

    # override
    def start_editing(self, run_script=True):
        pass

    # override
    def stop_editing(self, run_script=True):
        pass

    def get_context(self, cxt=None, action_cb=None):
        # get default menu context
        context = self.manager.get_context(action_cb)
        if isinstance(cxt, dict):
            context.update(cxt)
        return context

    def eval_enable(self):
        context = self.get_context()
        return MenuHelper.asbool(self._enable_tpl.render(context))

    # Called when a item is selected
    def select(self):
        self.__clear_scroll()

    def heartbeat(self, eventtime):
        self._last_heartbeat = eventtime
        state = bool(int(eventtime) & 1)
        if self.__last_state ^ state:
            self.__last_state = state
            if not self.is_editing():
                self._second_tick(eventtime)
                self.__update_scroll(eventtime)

    def __clear_scroll(self):
        self.__scroll_dir = None
        self.__scroll_diff = 0
        self.__scroll_offs = 0

    def __update_scroll(self, eventtime):
        if self.__scroll_dir == 0 and self.__scroll_diff > 0:
            self.__scroll_dir = 1
            self.__scroll_offs = 0
        elif self.__scroll_dir and self.__scroll_diff > 0:
            self.__scroll_offs += self.__scroll_dir
            if self.__scroll_offs >= self.__scroll_diff:
                self.__scroll_dir = -1
            elif self.__scroll_offs <= 0:
                self.__scroll_dir = 1
        else:
            self.__clear_scroll()

    def __render_scroll(self, s):
        if self.__scroll_dir is None:
            self.__scroll_dir = 0
            self.__scroll_offs = 0
        return s[
            self.__scroll_offs:self._width + self.__scroll_offs
        ].ljust(self._width)

    def render(self, scroll=False):
        s = str(self._render())
        if self._width > 0:
            self.__scroll_diff = len(s) - self._width
            if (scroll and self._scroll is True and self.is_scrollable()
                    and self.__scroll_diff > 0):
                s = self.__render_scroll(s)
            else:
                self.__clear_scroll()
                s = s[:self._width].ljust(self._width)
        else:
            self.__clear_scroll()
        return s

    def send_event(self, event, *args):
        return self.manager.send_event(
            "item:%s:%s" % (self.id, str(event)), *args)

    @property
    def manager(self):
        return self._manager

    @property
    def id(self):
        return self._id

    @property
    def ns(self):
        return self._ns


# menu container baseclass
class MenuContainer(MenuItem):
    def __init__(self, manager, config):
        super(MenuContainer, self).__init__(manager, config)
        self._show_back = MenuHelper.asbool(config.get('show_back', 'true'))
        self._show_title = MenuHelper.asbool(config.get('show_title', 'true'))
        self._allitems = []
        self._items = []

    def init(self):
        super(MenuContainer, self).init()
        # recursive guard
        self._parents = []

    # overload
    def _names_aslist(self):
        return []

    # overload
    def is_accepted(self, item):
        return isinstance(item, MenuItem)

    def is_readonly(self):
        return False

    def is_editing(self):
        return any([item.is_editing() for item in self._items])

    def stop_editing(self, run_script=True):
        for item in self._items:
            if item.is_editing():
                item.stop_editing(run_script)

    def lookup_item(self, item):
        if isinstance(item, str):
            s = item.strip()
            if s.startswith('.'):
                s = ' '.join([self.ns, s[1:]])
            item = self.manager.lookup_menuitem(s)
        return item

    # overload
    def _lookup_item(self, item):
        return self.lookup_item(item)

    def find_item(self, item, first_level=False):
        index = None
        if item in self._items:
            index = self._items.index(item)
        elif first_level is False:
            for con in self._items:
                if isinstance(con, MenuContainer) and item in con:
                    index = self._items.index(con)
        return index

    def add_parents(self, parents):
        if isinstance(parents, list):
            self._parents.extend(parents)
        else:
            self._parents.append(parents)

    def assert_recursive_relation(self, parents=None):
        assert self not in (parents or self._parents), \
            "Recursive relation of '%s' container" % (self.id,)

    def insert_item(self, s, index=None):
        item = self._lookup_item(s)
        if item is not None:
            if not self.is_accepted(item):
                raise error("Menu item '%s'is not accepted!" % str(type(item)))
            if isinstance(item, (MenuItem)):
                item.init()
            if isinstance(item, (MenuContainer)):
                item.add_parents(self._parents)
                item.add_parents(self)
                item.assert_recursive_relation()
                item.populate_items()
            if index is None:
                self._allitems.append(item)
            else:
                self._allitems.insert(index, item)

    # overload
    def _populate_items(self):
        pass

    def populate_items(self):
        self._allitems = []  # empty list
        for name in self._names_aslist():
            self.insert_item(name)
        # populate successor items
        self._populate_items()
        # send populate event
        self.send_event('populate', self)
        # insert back as first
        if self._show_back is True:
            name = '[..]'
            if self._show_title:
                name += ' %s' % str(self._name())
            self.insert_item(MenuCommand(self._manager, {
                'name': repr(name),
                'gcode': '{menu.back()}'
            }), 0)
        self.update_items()

    def update_items(self):
        self._items = [item for item in self._allitems if item.is_enabled()]

    def __iter__(self):
        return iter(self._items)

    def __len__(self):
        return len(self._items)

    def __getitem__(self, key):
        return self._items[key]


class MenuCommand(MenuItem):
    def __init__(self, manager, config):
        super(MenuCommand, self).__init__(manager, config)
        self._gcode_tpl = manager.gcode_macro.load_template(config, 'gcode')
        self._auto = MenuHelper.asbool(config.get('auto', 'true'))

    def is_readonly(self):
        return False

    def is_auto(self):
        return self._auto

    def get_gcode(self, action_cb=None):
        context = self.get_context(action_cb=action_cb)
        return self._gcode_tpl.render(context)

    def run_gcode(self):
        self.manager.queue_gcode(self.get_gcode())


class MenuInput(MenuCommand):
    def __init__(self, manager, config,):
        super(MenuInput, self).__init__(manager, config)
        self._reverse = MenuHelper.asbool(config.get('reverse', 'false'))
        self._realtime = MenuHelper.asbool(config.get('realtime', 'false'))
        self._readonly_tpl = manager.gcode_macro.load_template(
            config, 'readonly', 'False')
        self._input_tpl = manager.gcode_macro.load_template(config, 'input')
        self._input_min = config.getfloat('input_min', -999999.0)
        self._input_max = config.getfloat('input_max', 999999.0)
        self._input_step = config.getfloat('input_step', above=0.)
        self._input_step2 = config.getfloat('input_step2', 0, minval=0.)
        self._longpress_gcode_tpl = manager.gcode_macro.load_template(
            config, 'longpress_gcode', '')
        self._start_gcode_tpl = manager.gcode_macro.load_template(
            config, 'start_gcode', '')
        self._stop_gcode_tpl = manager.gcode_macro.load_template(
            config, 'stop_gcode', '')

    def _name(self):
        context = self.get_input_context()
        return MenuHelper.asliteral(MenuHelper.asflatline(
            self._name_tpl.render(context)))

    def init(self):
        super(MenuInput, self).init()
        self._is_dirty = False
        self.__last_change = None
        self._input_value = None
        self.__last_value = None

    def is_scrollable(self):
        return False

    def is_readonly(self):
        context = self.get_context()
        return MenuHelper.asbool(self._readonly_tpl.render(context))

    def is_realtime(self):
        return self._realtime

    def get_gcode(self, action_cb=None):
        context = self.get_input_context(action_cb=action_cb)
        return self._gcode_tpl.render(context)

    def get_longpress_gcode(self, action_cb=None):
        context = self.get_input_context(action_cb=action_cb)
        return self._longpress_gcode_tpl.render(context)

    def run_longpress_gcode(self):
        self.manager.queue_gcode(self.get_longpress_gcode())

    def run_start_gcode(self):
        context = self.get_input_context()
        self.manager.queue_gcode(self._start_gcode_tpl.render(context))

    def run_stop_gcode(self):
        context = self.get_input_context()
        self.manager.queue_gcode(self._stop_gcode_tpl.render(context))

    def is_editing(self):
        return self._input_value is not None

    def stop_editing(self, run_script=True):
        if run_script is True:
            self.run_stop_gcode()
        self._reset_value()

    def start_editing(self, run_script=True):
        self._init_value()
        if run_script is True:
            self.run_start_gcode()

    def get_value(self):
        return self._input_value

    def heartbeat(self, eventtime):
        super(MenuInput, self).heartbeat(eventtime)
        if (self._realtime
                and not self.is_readonly()
                and self._is_dirty is True
                and self.__last_change is not None
                and self._input_value is not None
                and (eventtime - self.__last_change) > 0.200):
            self.run_gcode()
            self._is_dirty = False

    def get_input_context(self, cxt=None, action_cb=None):
        context = self.get_context({
            'input': MenuHelper.asfloat(
                self._eval_value() if self._input_value is None
                else self._input_value)
        }, action_cb=action_cb)
        if isinstance(cxt, dict):
            context.update(cxt)
        return context

    def _eval_value(self):
        return self._input_tpl.render(self.get_context())

    def _value_changed(self):
        self.__last_change = self._last_heartbeat
        self._is_dirty = True

    def _init_value(self):
        self._input_value = None
        self.__last_value = None
        if not self.is_readonly():
            value = self._eval_value()
            if MenuHelper.isfloat(value):
                self._input_value = min(self._input_max, max(
                    self._input_min, MenuHelper.asfloat(value)))
                if self._realtime:
                    self._value_changed()
            else:
                logging.error("Cannot init input value")

    def _reset_value(self):
        self._input_value = None

    def inc_value(self, fast_rate=False):
        last_value = self._input_value
        input_step = (self._input_step2 if fast_rate and self._input_step2 > 0
                      else self._input_step)
        if self._input_value is None:
            return

        if(self._reverse is True):
            self._input_value -= abs(input_step)
        else:
            self._input_value += abs(input_step)
        self._input_value = min(self._input_max, max(
            self._input_min, self._input_value))

        if self._realtime and last_value != self._input_value:
            self._value_changed()

    def dec_value(self, fast_rate=False):
        last_value = self._input_value
        input_step = (self._input_step2 if fast_rate and self._input_step2 > 0
                      else self._input_step)
        if self._input_value is None:
            return

        if(self._reverse is True):
            self._input_value += abs(input_step)
        else:
            self._input_value -= abs(input_step)
        self._input_value = min(self._input_max, max(
            self._input_min, self._input_value))

        if self._realtime and last_value != self._input_value:
            self._value_changed()


class MenuGroup(MenuContainer):
    def __init__(self, manager, config, sep=','):
        super(MenuGroup, self).__init__(manager, config)
        self._sep = sep
        self._show_back = False
        self.use_cursor = MenuHelper.asbool(config.get('use_cursor', 'false'))
        self.items = config.get('items', '')

    def init(self):
        super(MenuGroup, self).init()
        self.selected = None
        self._leaving_dir = None  # 0 - bottom, 1 - top, None - undefined

    def is_accepted(self, item):
        return (super(MenuGroup, self).is_accepted(item)
                and type(item) is not MenuCard)

    def is_scrollable(self):
        return False

    def is_enabled(self):
        return not not len(self)

    def is_readonly(self):
        return all([item.is_readonly() for item in self._items])

    def _names_aslist(self):
        return MenuHelper.words_aslist(self.items, sep=self._sep)

    def select(self):
        super(MenuGroup, self).select()
        for item in self._items:
            item.select()

    def _render_item(self, item, selected=False, scroll=False):
        name = "%s" % str(item.render(scroll))
        if selected and not self.is_editing():
            if self.use_cursor:
                name = (item.cursor if isinstance(item, MenuItem)
                        else MenuCursor.SELECT) + name
            else:
                name = (name if self.manager.blink_slow_state
                        else ' '*len(name))
        elif selected and self.is_editing():
            if self.use_cursor:
                name = MenuCursor.EDIT + name
            else:
                name = (name if self.manager.blink_fast_state
                        else ' '*len(name))
        elif self.use_cursor:
            name = MenuCursor.NONE + name
        return name

    def _render(self):
        s = ""
        if self.selected is not None:
            self.selected = (
                (self.selected % len(self)) if len(self) > 0 else None)

        for i, item in enumerate(self):
            s += self._render_item(item, (i == self.selected), True)
        return s

    def _call_selected(self, method=None, *args):
        res = None
        if self.selected is not None:
            try:
                if method is None:
                    res = self[self.selected]
                else:
                    res = getattr(self[self.selected], method)(*args)
            except Exception:
                logging.exception("Call selected error")
        return res

    def stop_editing(self, run_script=True):
        return self._call_selected('stop_editing', run_script)

    def start_editing(self, run_script=True):
        return self._call_selected('start_editing', run_script)

    def is_editing(self):
        return self._call_selected('is_editing')

    def inc_value(self, fast_rate=False):
        self._call_selected('inc_value', fast_rate)

    def dec_value(self, fast_rate=False):
        self._call_selected('dec_value', fast_rate)

    def selected_item(self):
        return self._call_selected()

    def select_item(self, needle):
        if isinstance(needle, MenuItem) and not needle.is_readonly():
            if self._call_selected() is not needle:
                self.selected = None
                self._leaving_dir = None
                for i, item in enumerate(self):
                    if item is needle:
                        self.selected = i
                        break
        else:
            logging.error("Cannot select read-only item")
        return self.selected

    def find_next_item(self):
        if self.selected is None:
            self.selected = 0
        elif self.selected < len(self) - 1:
            self.selected += 1
        else:
            self._leaving_dir = 1
            self.selected = None
        # skip readonly
        while (self.selected is not None
                and self.selected < len(self)
                and self._call_selected('is_readonly')):
            if self.selected < len(self) - 1:
                self.selected = (self.selected + 1)
            else:
                self._leaving_dir = 1
                self.selected = None
        return self.selected

    def find_prev_item(self):
        if self.selected is None:
            self.selected = len(self) - 1
        elif self.selected > 0:
            self.selected -= 1
        else:
            self._leaving_dir = 0
            self.selected = None
        # skip readonly
        while (self.selected is not None
                and self.selected >= 0
                and self._call_selected('is_readonly')):
            if self.selected > 0:
                self.selected = (self.selected - 1)
            else:
                self._leaving_dir = 0
                self.selected = None
        return self.selected


class MenuItemGroup(MenuGroup):
    def __init__(self, manager, config, sep='|'):
        super(MenuItemGroup, self).__init__(manager, config, sep)

    def is_readonly(self):
        return True

    def is_accepted(self, item):
        return type(item) is MenuItem


class MenuCycler(MenuGroup):
    def __init__(self, manager, config, sep=','):
        super(MenuCycler, self).__init__(manager, config, sep)

    def init(self):
        super(MenuCycler, self).init()
        self._interval = 0
        self.__interval_cnt = 0
        self.__alllen = 0
        self._curr_idx = 0

    def is_readonly(self):
        return True

    def is_accepted(self, item):
        return type(item) in (MenuItem, MenuItemGroup)

    def _lookup_item(self, item):
        if isinstance(item, str) and '|' in item:
            item = MenuItemGroup(self.manager, {
                '__ns': self.ns,
                'name': repr(' '.join([self.id, 'ItemGroup'])),
                'items': item
            }, '|')
        elif isinstance(item, str) and item.isdigit():
            try:
                self._interval = max(0, int(item))
            except Exception:
                logging.exception("Interval parsing error")
            item = None
        return super(MenuCycler, self)._lookup_item(item)

    def _second_tick(self, eventtime):
        super(MenuCycler, self)._second_tick(eventtime)
        if self._interval > 0:
            self.__interval_cnt = (self.__interval_cnt + 1) % self._interval
            if self.__interval_cnt == 0 and self.__alllen > 0:
                self._curr_idx = (self._curr_idx + 1) % self.__alllen
        else:
            self._curr_idx = 0

    def heartbeat(self, eventtime):
        super(MenuCycler, self).heartbeat(eventtime)
        for item in self._items:
            item.heartbeat(eventtime)

    def update_items(self):
        items = [item for item in self._allitems if item.is_enabled()]
        self.__alllen = len(items)
        if self.__alllen > 0:
            self._curr_idx = self._curr_idx % self.__alllen
            self._items = [items[self._curr_idx]]
        else:
            self._curr_idx = 0
            self._items = []


class MenuList(MenuContainer):
    def __init__(self, manager, config):
        super(MenuList, self).__init__(manager, config)
        self._enter_gcode_tpl = manager.gcode_macro.load_template(
            config, 'enter_gcode', '')
        self._leave_gcode_tpl = manager.gcode_macro.load_template(
            config, 'leave_gcode', '')
        self.items = config.get('items', '')

    def is_accepted(self, item):
        return (super(MenuList, self).is_accepted(item)
                and type(item) is not MenuCard)

    def _names_aslist(self):
        return MenuHelper.lines_aslist(self.items)

    def _lookup_item(self, item):
        if isinstance(item, str) and ',' in item:
            item = MenuGroup(self.manager, {
                '__ns': self.ns,
                'name': repr(' '.join([self.id, 'Group'])),
                'items': item
            }, ',')
        return super(MenuList, self)._lookup_item(item)

    def update_items(self):
        super(MenuList, self).update_items()
        for item in self._items:
            if isinstance(item, MenuGroup) and not item.is_editing():
                item.update_items()

    def run_enter_gcode(self):
        context = self.get_context()
        self.manager.queue_gcode(self._enter_gcode_tpl.render(context))

    def run_leave_gcode(self):
        context = self.get_context()
        self.manager.queue_gcode(self._leave_gcode_tpl.render(context))


class MenuVSDCard(MenuList):
    def __init__(self, manager, config):
        super(MenuVSDCard, self).__init__(manager, config)

    def _populate_items(self):
        super(MenuVSDCard, self)._populate_items()
        sdcard = self.manager.objs.get('virtual_sdcard')
        if sdcard is not None:
            files = sdcard.get_file_list()
            for fname, fsize in files:
                gcode = [
                    'M23 /%s' % str(fname)
                ]
                self.insert_item(MenuCommand(self.manager, {
                    'name': repr('%s' % str(fname)),
                    'cursor': '+',
                    'gcode': "\n".join(gcode),
                    'scroll': True
                }))


class MenuCard(MenuGroup):
    def __init__(self, manager, config):
        super(MenuCard, self).__init__(manager, config)
        prfx = 'content_'
        self._content_tpls = [manager.gcode_macro.load_template(
            config, o) for o in config.get_prefix_options(prfx)]
        self.sticky = config.get('sticky', None)
        self.inline_ns_prefix = config.get(
            'inline_ns_prefix', '__ns__')
        self._sticky = None
        self._inline_names = []
        # find all template variables (except variable 'items')
        self._tpl_vars = [(n, self._ns_prefix(n)) for t in self._content_tpls
                          for n in t.find_variables() if n != 'items']

    def _ns_prefix(self, name):
        name = str(name).strip()
        if self.inline_ns_prefix and \
                name.startswith(self.inline_ns_prefix):
            name = ' '.join([self.ns, name[
                len(self.inline_ns_prefix):]])
        return name

    def _names_aslist(self):
        return MenuHelper.lines_aslist(self.items)

    def update_items(self):
        self._items = self._allitems[:]
        for item in self._items:
            if isinstance(item, MenuGroup) and not item.is_editing():
                item.update_items()

    def _lookup_item(self, item):
        if isinstance(item, str) and ',' in item:
            item = MenuCycler(self.manager, {
                '__ns': self.ns,
                'name': repr(' '.join([self.id, 'Cycler'])),
                'items': item
            }, ',')
        return super(MenuCard, self)._lookup_item(item)

    def _lookup_sticky(self):
        self._sticky = None
        if self.sticky is not None:
            item = self.lookup_item(self.sticky)
            if isinstance(item, MenuItem) and not item.is_readonly():
                self._sticky = item
            else:
                logging.error("Cannot stick to read-only item")

    def populate_inline_items(self):
        self._inline_names = []
        menuitems = [n for n, m in self.manager.lookup_menuitems()]
        for var_name, cfg_name in self._tpl_vars:
            if cfg_name in menuitems:
                self.insert_item(cfg_name)
                self._inline_names.append(var_name)
        self.update_items()

    def _populate_items(self):
        super(MenuCard, self)._populate_items()
        self.populate_inline_items()
        self._lookup_sticky()

    def select(self):
        super(MenuCard, self).select()
        if self._sticky is not None:
            self.selected = None
            self._leaving_dir = None

    def render_content(self, eventtime, constrained=False):
        rendered_items = []
        rendered_kwitems = {}

        if self.selected is not None:
            self.selected = (
                (self.selected % len(self)) if len(self) > 0 else None)
        else:
            if (self._leaving_dir is None
                    and self._sticky is not None
                    and self.select_item(self._sticky) is not None):
                if (isinstance(self._sticky, MenuInput)
                        and not self._sticky.is_editing()):
                    if self.is_editing():
                        self.stop_editing()
                    self._sticky.start_editing()
            elif self._leaving_dir == 0 and len(self) > 0:
                self.find_next_item()
            elif self._leaving_dir == 1 and len(self) > 0:
                self.find_prev_item()
            elif constrained is True:
                self.find_next_item()
            else:
                self.selected = None

        nitems = len(self._names_aslist())
        for i, item in enumerate(self):
            s = ''
            if item.is_enabled():
                item.heartbeat(eventtime)
                s = self._render_item(item, (i == self.selected), True)
            rendered_items.append(s)
            if self._inline_names and i >= nitems:
                rendered_kwitems[self._inline_names[i-nitems]] = s

        context = self.get_context(
            dict({'items': tuple(rendered_items)}, **rendered_kwitems))
        lines = []
        for tpl in self._content_tpls:
            try:
                lines.append(MenuHelper.asliteral(MenuHelper.asflatline(
                    tpl.render(context))))
            except Exception:
                logging.exception('Card rendering error')
        return lines

    def _render(self):
        return self._name()


class MenuDeck(MenuList):
    def __init__(self, manager, config):
        super(MenuDeck, self).__init__(manager, config)
        self.menu = config.get('longpress_menu', None)
        self.constrained = MenuHelper.asbool(
            config.get('constrained', 'false'))
        self._menu = None
        self._show_back = False
        self._show_title = False
        if not self.items:
            id = " ".join([self.id, "__singlecard__"])
            cfg = {o: config.get(o) for o in config.get_prefix_options('')}
            cfg['__id'] = id
            cfg['__ns'] = self.ns
            self.manager.add_menuitem(id, MenuCard(self.manager, cfg))
            self.items = id

    def is_constrained(self):
        return self.constrained

    def get_longpress_menu(self):
        return self._menu

    def _populate_menu(self):
        self._menu = None
        if self.menu is not None:
            menu = self.manager.lookup_menuitem(self.menu)
            if isinstance(menu, MenuContainer):
                menu.assert_recursive_relation(self._parents)
                menu.populate_items()
                self._menu = menu

    def _populate_items(self):
        super(MenuDeck, self)._populate_items()
        self._populate_menu()

    def _names_aslist(self):
        return MenuHelper.aslist(self.items)

    def is_accepted(self, item):
        return type(item) is MenuCard

    def _render(self):
        return self._name()


menu_items = {
    'item': MenuItem,
    'command': MenuCommand,
    'input': MenuInput,
    'list': MenuList,
    'vsdcard': MenuVSDCard,
    'deck': MenuDeck,
    'card': MenuCard
}

MENU_UPDATE_DELAY = .100
TIMER_DELAY = .200
LONG_PRESS_DURATION = 0.800
BLINK_FAST_SEQUENCE = (True, True, False, False)
BLINK_SLOW_SEQUENCE = (True, True, True, True, False, False, False)


class MenuManager:
    def __init__(self, config, display):
        self.running = False
        self.menuitems = {}
        self.menustack = []
        self._autorun = False
        self.top_row = 0
        self.selected = 0
        self.blink_fast_state = True
        self.blink_slow_state = True
        self._last_eventtime = 0
        self.blink_fast_idx = 0
        self.blink_slow_idx = 0
        self.timeout_idx = 0
        self.display = display
        self.lcd_chip = display.get_lcd_chip()
        self.printer = config.get_printer()
        self.pconfig = self.printer.lookup_object('configfile')
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode_queue = []
        self.parameters = {}
        self.objs = {}
        self.root = None
        self._root = config.get('menu_root', '__main')
        self.cols, self.rows = self.lcd_chip.get_dimensions()
        self.timeout = config.getint('menu_timeout', 0)
        self.timer = 0
        # buttons
        self.encoder_pins = config.get('encoder_pins', None)
        self.click_pin = config.get('click_pin', None)
        self.back_pin = config.get('back_pin', None)
        self.up_pin = config.get('up_pin', None)
        self.down_pin = config.get('down_pin', None)
        self.kill_pin = config.get('kill_pin', None)
        # analog button ranges
        self.analog_range_click_pin = config.get(
            'analog_range_click_pin', None)
        self.analog_range_back_pin = config.get(
            'analog_range_back_pin', None)
        self.analog_range_up_pin = config.get(
            'analog_range_up_pin', None)
        self.analog_range_down_pin = config.get(
            'analog_range_down_pin', None)
        self.analog_range_kill_pin = config.get(
            'analog_range_kill_pin', None)
        self._last_click_press = 0
        self.analog_pullup = config.getfloat(
            'analog_pullup_resistor', 4700., above=0.)
        self.analog_pin_debug = config.getboolean('analog_pin_debug', False)
        self._encoder_fast_rate = config.getfloat(
            'encoder_fast_rate', .03, above=0.)
        self._last_encoder_cw_eventtime = 0
        self._last_encoder_ccw_eventtime = 0
        # printer objects
        self.buttons = self.printer.try_load_module(config, "buttons")
        self.gcode_macro = self.printer.try_load_module(config, 'gcode_macro')
        # register itself for printer callbacks
        self.printer.add_object('menu', self)
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        # register buttons & encoder
        if self.buttons:
            # digital buttons
            if self.encoder_pins:
                try:
                    pin1, pin2 = self.encoder_pins.split(',')
                except Exception:
                    raise config.error("Unable to parse encoder_pins")
                self.buttons.register_rotary_encoder(
                    pin1.strip(), pin2.strip(),
                    self.encoder_cw_callback, self.encoder_ccw_callback)
            if self.click_pin:
                if self.analog_range_click_pin is not None:
                    try:
                        p_min, p_max = map(
                            float, self.analog_range_click_pin.split(','))
                    except Exception:
                        raise config.error(
                            "Unable to parse analog_range_click_pin")
                    self.buttons.register_adc_button(
                        self.click_pin, p_min, p_max, self.analog_pullup,
                        self.click_callback, self.analog_pin_debug)
                else:
                    self.buttons.register_buttons(
                        [self.click_pin], self.click_callback)
            if self.back_pin:
                if self.analog_range_back_pin is not None:
                    try:
                        p_min, p_max = map(
                            float, self.analog_range_back_pin.split(','))
                    except Exception:
                        raise config.error(
                            "Unable to parse analog_range_back_pin")
                    self.buttons.register_adc_button_push(
                        self.back_pin, p_min, p_max, self.analog_pullup,
                        self.back_callback, self.analog_pin_debug)
                else:
                    self.buttons.register_button_push(
                        self.back_pin, self.back_callback)
            if self.up_pin:
                if self.analog_range_up_pin is not None:
                    try:
                        p_min, p_max = map(
                            float, self.analog_range_up_pin.split(','))
                    except Exception:
                        raise config.error(
                            "Unable to parse analog_range_up_pin")
                    self.buttons.register_adc_button_push(
                        self.up_pin, p_min, p_max, self.analog_pullup,
                        self.up_callback, self.analog_pin_debug)
                else:
                    self.buttons.register_button_push(
                        self.up_pin, self.up_callback)
            if self.down_pin:
                if self.analog_range_down_pin is not None:
                    try:
                        p_min, p_max = map(
                            float, self.analog_range_down_pin.split(','))
                    except Exception:
                        raise config.error(
                            "Unable to parse analog_range_down_pin")
                    self.buttons.register_adc_button_push(
                        self.down_pin, p_min, p_max, self.analog_pullup,
                        self.down_callback, self.analog_pin_debug)
                else:
                    self.buttons.register_button_push(
                        self.down_pin, self.down_callback)
            if self.kill_pin:
                if self.analog_range_kill_pin is not None:
                    try:
                        p_min, p_max = map(
                            float, self.analog_range_kill_pin.split(','))
                    except Exception:
                        raise config.error(
                            "Unable to parse analog_range_kill_pin")
                    self.buttons.register_adc_button_push(
                        self.kill_pin, p_min, p_max, self.analog_pullup,
                        self.kill_callback, self.analog_pin_debug)
                else:
                    self.buttons.register_button_push(
                        self.kill_pin, self.kill_callback)

        # Load local config file in same directory as current module
        self.load_config(os.path.dirname(__file__), 'menu.cfg')
        # Load items from main config
        self.load_menuitems(config)
        # Load menu root
        self.load_root()
        # send init event
        self.send_event('init', self)

    def handle_ready(self):
        # start timer
        reactor = self.printer.get_reactor()
        reactor.register_timer(self.timer_event, reactor.NOW)

    def timer_event(self, eventtime):
        self._last_eventtime = eventtime
        # take next from sequence
        self.blink_fast_idx = (
            (self.blink_fast_idx + 1) % len(BLINK_FAST_SEQUENCE)
        )
        self.blink_slow_idx = (
            (self.blink_slow_idx + 1) % len(BLINK_SLOW_SEQUENCE)
        )
        self.timeout_idx = (self.timeout_idx + 1) % 5  # 0.2*5 = 1s
        self.blink_fast_state = (
            not not BLINK_FAST_SEQUENCE[self.blink_fast_idx]
        )
        self.blink_slow_state = (
            not not BLINK_SLOW_SEQUENCE[self.blink_slow_idx]
        )
        if self.timeout_idx == 0:
            self.timeout_check(eventtime)
        # check long press
        if (self._last_click_press > 0 and (
                eventtime - self._last_click_press) >= LONG_PRESS_DURATION):
            # long click
            self._last_click_press = 0
            self._long_click_callback(eventtime)
        return eventtime + TIMER_DELAY

    def timeout_check(self, eventtime):
        # check timeout
        if (self.is_running() and self.timeout > 0
                and self.root is not None
                and self._allow_timeout()):
            if self.timer >= self.timeout:
                self.exit()
            else:
                self.timer += 1
        else:
            self.timer = 0

    def _allow_timeout(self):
        container = self.stack_peek()
        if (container is self.root
                and self.selected == 0
                and self._autorun is True):
            current = container[0] if len(container) > 0 else None
            if (isinstance(container, MenuDeck)
                    and isinstance(current, MenuCard)
                    and container.is_constrained() is False
                    and current.selected_item() is not None):
                return True
            return False
        return True

    def restart(self, root=None, force_exit=True):
        if self.is_running():
            self.exit(force_exit)
        self.load_root(root)

    def load_root(self, root=None, autorun=False):
        root = self._root if root is None else root
        if root is not None:
            self.root = self.lookup_menuitem(root)
            if isinstance(self.root, MenuDeck):
                self._autorun = True
            else:
                self._autorun = autorun

    def register_object(self, obj, name=None, override=False):
        """Register an object with a "get_status" callback"""
        if obj is not None:
            if name is None:
                name = obj.__class__.__name__
            if override or name not in self.objs:
                self.objs[name] = obj

    def unregister_object(self, name):
        """Unregister an object from "get_status" callback list"""
        if name is not None:
            if not isinstance(name, str):
                name = name.__class__.__name__
            if name in self.objs:
                self.objs.pop(name)

    def after(self, starttime, callback, *args):
        """Helper method for reactor.register_callback.
        The callback will be executed once after the start time elapses.
        Starttime values less than 3600 are considered as timeout/delay seconds
        from current reactor time."""
        def callit(eventtime):
            callback(eventtime, *args)
        reactor = self.printer.get_reactor()
        starttime = max(0., float(starttime))
        if starttime < 3600.0:  # 1h
            starttime = reactor.monotonic() + starttime
        reactor.register_callback(callit, starttime)

    def send_event(self, event, *args):
        return self.printer.send_event("menu:" + str(event), *args)

    def is_running(self):
        return self.running

    def begin(self, eventtime):
        self.menustack = []
        self.top_row = 0
        self.selected = 0
        self.timer = 0
        if isinstance(self.root, MenuContainer):
            # send begin event
            self.send_event('begin', self)
            self.update_parameters(eventtime)
            self.root.populate_items()
            self.stack_push(self.root)
            self.running = True
            return
        elif self.root is not None:
            logging.error("Invalid root '%s', menu stopped!" % str(self._root))

        self.running = False

    def get_status(self, eventtime):
        return {
            'eventtime': eventtime,
            'timeout': self.timeout,
            'autorun': self._autorun,
            'running': self.running,
            'blink_fast': self.blink_fast_state,
            'blink_slow': self.blink_slow_state,
            'rows': self.rows,
            'cols': self.cols
        }

    def get_context(self, action_cb=None):
        def action(name, *a):
            if callable(action_cb):
                return lambda *b: action_cb(name, *(a+b))
            else:
                return lambda *b: ''

        # menu default jinja2 context
        return {
            'printer': self.parameters,
            's2days': MenuHelper.seconds2('days'),
            's2hours': MenuHelper.seconds2('hours'),
            's2mins': MenuHelper.seconds2('minutes'),
            's2secs': MenuHelper.seconds2('seconds'),
            'lerp': MenuHelper.interpolate,
            'seq': MenuHelper.sequence(self._last_eventtime),
            'menu': {
                'back': action('menu-back'),
                'exit': action('menu-exit'),
                'emit': action('menu-emit'),
                'log': action('menu-log')
            },
            'deck': {
                'menu': action('deck', 'menu')
            },
            'editing': {
                'start': action('editing', 'start'),
                'stop': action('editing', 'stop')
            },
            'run': {
                'gcode': action('run-gcode'),
                'longpress_gcode': action('run-longpress-gcode')
            }
        }

    def update_parameters(self, eventtime):
        self.parameters = self.gcode_macro.create_status_wrapper(eventtime)
        objs = dict(self.objs)
        for name in objs.keys():
            try:
                if objs[name] is not None:
                    get_status = getattr(objs[name], "get_status", None)
                    if callable(get_status):
                        self.parameters[name] = get_status(eventtime)
                    else:
                        self.parameters[name] = {}
            except Exception:
                logging.exception("Parameter '%s' update error" % str(name))

    def stack_push(self, container):
        if not isinstance(container, MenuContainer):
            raise error("Wrong type, expected MenuContainer")
        top = self.stack_peek()
        if top is not None:
            top.run_leave_gcode()
        container.run_enter_gcode()
        if not container.is_editing():
            container.update_items()
        self.menustack.append(container)

    def stack_pop(self):
        container = None
        if self.stack_size() > 0:
            container = self.menustack.pop()
            if not isinstance(container, MenuContainer):
                raise error("Wrong type, expected MenuContainer")
            top = self.stack_peek()
            if top is not None:
                if not isinstance(container, MenuContainer):
                    raise error("Wrong type, expected MenuContainer")
                if not top.is_editing():
                    top.update_items()
                container.run_leave_gcode()
                top.run_enter_gcode()
            else:
                container.run_leave_gcode()
        return container

    def stack_size(self):
        return len(self.menustack)

    def stack_peek(self, lvl=0):
        container = None
        if self.stack_size() > lvl:
            container = self.menustack[self.stack_size() - lvl - 1]
        return container

    def render(self, eventtime):
        lines = []
        self.update_parameters(eventtime)
        container = self.stack_peek()
        if self.running and isinstance(container, MenuContainer):
            container.heartbeat(eventtime)
            if(isinstance(container, MenuDeck) and not container.is_editing()):
                container.update_items()
            # clamps
            self.top_row = max(0, min(
                self.top_row, len(container) - self.rows))
            self.selected = max(0, min(
                self.selected, len(container) - 1))
            if isinstance(container, MenuDeck):
                container[self.selected].heartbeat(eventtime)
                lines = container[self.selected].render_content(
                    eventtime, container.is_constrained())
            else:
                for row in range(self.top_row, self.top_row + self.rows):
                    s = ""
                    if row < len(container):
                        selected = (row == self.selected)
                        current = container[row]
                        if selected:
                            current.heartbeat(eventtime)
                            if (isinstance(current, (MenuInput, MenuGroup))
                                    and current.is_editing()):
                                s += MenuCursor.EDIT
                            elif isinstance(current, MenuItem):
                                s += current.cursor
                            else:
                                s += MenuCursor.SELECT
                        else:
                            s += MenuCursor.NONE

                        name = "%s" % str(current.render(selected))
                        i = len(s)
                        if isinstance(current, MenuList):
                            s += name[:self.cols-i-1].ljust(self.cols-i-1)
                            s += '>'
                        else:
                            s += name[:self.cols-i].ljust(self.cols-i)
                    lines.append(s.ljust(self.cols))
        return lines

    def screen_update_event(self, eventtime):
        if self.is_running():
            self.lcd_chip.clear()
            for y, line in enumerate(self.render(eventtime)):
                self.display.draw_text(0, y, line)
            self.lcd_chip.flush()
            return eventtime + MENU_UPDATE_DELAY
        elif not self.is_running() and self._autorun is True:
            # lets start and populate the menu items
            self.begin(eventtime)
            return eventtime + MENU_UPDATE_DELAY
        else:
            return 0

    def up(self, fast_rate=False):
        container = self.stack_peek()
        if self.running and isinstance(container, MenuContainer):
            self.timer = 0
            current = container[self.selected]
            if (isinstance(current, (MenuInput, MenuGroup))
                    and current.is_editing()):
                current.dec_value(fast_rate)
            elif (isinstance(current, MenuGroup)
                    and current.find_prev_item() is not None):
                pass
            else:
                if self.selected == 0:
                    return
                if self.selected > self.top_row:
                    self.selected -= 1
                else:
                    self.top_row -= 1
                    self.selected -= 1
                # init element
                if isinstance(container[self.selected], MenuItem):
                    container[self.selected].select()
                # wind up group last item or init item
                if (isinstance(container[self.selected], MenuGroup)
                        and type(container[self.selected]) is not MenuCard):
                    container[self.selected].find_prev_item()

    def down(self, fast_rate=False):
        container = self.stack_peek()
        if self.running and isinstance(container, MenuContainer):
            self.timer = 0
            current = container[self.selected]
            if (isinstance(current, (MenuInput, MenuGroup))
                    and current.is_editing()):
                current.inc_value(fast_rate)
            elif (isinstance(current, MenuGroup)
                    and current.find_next_item() is not None):
                pass
            else:
                if self.selected >= len(container) - 1:
                    return
                if self.selected < self.top_row + self.rows - 1:
                    self.selected += 1
                else:
                    self.top_row += 1
                    self.selected += 1
                # init element
                if isinstance(container[self.selected], MenuItem):
                    container[self.selected].select()
                # wind up group first item
                if (isinstance(container[self.selected], MenuGroup)
                        and type(container[self.selected]) is not MenuCard):
                    container[self.selected].find_next_item()

    def back(self):
        container = self.stack_peek()
        if self.running and isinstance(container, MenuContainer):
            self.timer = 0
            current = container[self.selected]
            if (isinstance(current, (MenuInput, MenuGroup))
                    and current.is_editing()):
                return
            parent = self.stack_peek(1)
            if isinstance(parent, MenuContainer):
                self.stack_pop()
                index = parent.find_item(container)
                if index is not None and index < len(parent):
                    self.top_row = index
                    self.selected = index
                else:
                    self.top_row = 0
                    self.selected = 0
                # init element
                if isinstance(parent[self.selected], MenuItem):
                    parent[self.selected].select()
                # wind up group first item or init item
                if (isinstance(parent[self.selected], MenuGroup)
                        and type(parent[self.selected]) is not MenuCard):
                    parent[self.selected].find_next_item()
            else:
                self.stack_pop()
                self.running = False

    def exit(self, force=False):
        container = self.stack_peek()
        if self.running and isinstance(container, MenuContainer):
            current = container[self.selected]
            if (not force and isinstance(current, (MenuInput, MenuGroup))
                    and current.is_editing()):
                return
            container.run_leave_gcode()
            self.send_event('exit', self)
            self.running = False

    def process_actions(self, actions, names, source):
        matches = None
        if (actions and isinstance(actions, list)
                and names and isinstance(names, list)):
            # Process matching actions
            matches = [t for t in actions if t[1] in names]
            for match in matches:
                i, name, args = match
                malformed = False
                # remove found actions from global action list
                actions.remove(match)
                # process found actions callback
                if name == 'menu-back':
                    self.back()
                elif name == 'menu-exit':
                    self.exit()
                elif name == 'deck':
                    if len(args[0:]) > 0:
                        if args[0] == 'menu':
                            self.push_deck_menu()
                    else:
                        malformed = True
                elif name == 'editing':
                    run_script = True
                    if len(args[0:]) > 0:
                        if len(args[1:]) > 0:
                            run_script = MenuHelper.asbool(args[1])
                        if args[0] == 'stop':
                            if (isinstance(source, MenuInput)
                                    and source.is_editing()):
                                source.stop_editing(run_script)
                        elif args[0] == 'start':
                            if (isinstance(source, MenuInput)
                                    and not source.is_editing()):
                                source.start_editing(run_script)
                        else:
                            malformed = True
                    else:
                        malformed = True
                elif name == 'run-gcode':
                    if (isinstance(source, MenuInput) and source.is_editing()):
                        source.run_gcode()
                    elif (isinstance(source, MenuCommand)):
                        source.run_gcode()
                elif name == 'run-longpress-gcode':
                    if (isinstance(source, MenuInput) and source.is_editing()):
                        source.run_longpress_gcode()
                elif name == 'menu-emit':
                    if len(args[0:]) > 0 and len(str(args[0])) > 0:
                        self.send_event(
                            "action:" + str(args[0]), source, *args[1:])
                    else:
                        malformed = True
                elif name == 'menu-log':
                    if len(args[0:]) > 0:
                        logging.info("menu:{} {}".format(
                            repr(source), ' '.join(map(str, args[0:]))))
                    else:
                        malformed = True
                else:
                    logging.error("Unknown action: {} {}".format(
                        name, ' '.join(map(str, args[0:]))))
                if malformed is True:
                    logging.error("Malformed action: {} {}".format(
                        name, ' '.join(map(str, args[0:]))))
        return matches

    def enter(self, long_press=False):
        actions = []

        def cb(name, *args):
            actions.append((len(actions), name, list(args)))
            return ''

        container = self.stack_peek()
        if self.running and isinstance(container, MenuContainer):
            self.timer = 0
            current = container[self.selected]
            if isinstance(current, MenuGroup):
                current = current.selected_item()
            if isinstance(current, MenuList):
                self.stack_push(current)
                self.top_row = 0
                self.selected = 0
            elif isinstance(current, MenuInput):
                if current.is_editing():
                    if long_press is True:
                        gcode = current.get_longpress_gcode(action_cb=cb)
                        if current.is_auto() is True:
                            self.queue_gcode(gcode)
                        else:
                            self.process_actions(
                                actions, ['run-longpress-gcode'], current)
                    else:
                        gcode = current.get_gcode(action_cb=cb)
                        if current.is_auto() is True:
                            if not current.is_realtime():
                                self.queue_gcode(gcode)
                            current.stop_editing()
                        else:
                            self.process_actions(
                                actions, ['run-gcode'], current)
                    self.process_actions(actions, ['editing'], current)
                else:
                    current.start_editing()
            elif isinstance(current, MenuCommand):
                gcode = current.get_gcode(action_cb=cb)
                if current.is_auto() is True:
                    self.queue_gcode(gcode)
                else:
                    self.process_actions(actions, ['run-gcode'], current)
            # process actions
            self.process_actions(actions, [
                'menu-back', 'menu-exit', 'deck', 'menu-emit', 'menu-log'
            ], current)

    def queue_gcode(self, script):
        if script is None:
            return
        if not self.gcode_queue:
            reactor = self.printer.get_reactor()
            reactor.register_callback(self.dispatch_gcode)
        self.gcode_queue.append(script)

    def dispatch_gcode(self, eventtime):
        while self.gcode_queue:
            script = self.gcode_queue[0]
            try:
                self.gcode.run_script(script)
            except Exception:
                logging.exception("Script running error")
            self.gcode_queue.pop(0)

    def menuitem_from_config(self, config):
        return MenuHelper.aschoice(
            config, 'type', menu_items)(self, config)

    def add_menuitem(self, name, menu):
        if name in self.menuitems:
            logging.info(
                "Declaration of '%s' hides "
                "previous menuitem declaration" % (name,))
        self.menuitems[name] = menu

    def lookup_menuitem(self, name, default=sentinel):
        if name is None:
            return None
        if name in self.menuitems:
            return self.menuitems[name]
        if default is sentinel:
            raise self.printer.config_error(
                "Unknown menuitem '%s'" % (name,))
        return default

    def lookup_menuitems(self, prefix=None):
        if prefix is None:
            return list(self.menuitems.items())
        items = [(n, self.menuitems[n])
                 for n in self.menuitems if n.startswith(prefix + ' ')]
        if prefix in self.menuitems:
            return [(prefix, self.menuitems[prefix])] + items
        return items

    def load_config(self, *args):
        cfg = None
        filename = os.path.join(*args)
        try:
            cfg = self.pconfig.read_config(filename)
        except Exception:
            raise self.printer.config_error(
                "Cannot load config '%s'" % (filename,))
        if cfg:
            self.load_menuitems(cfg)
        return cfg

    def load_menuitems(self, config):
        for cfg in config.get_prefix_sections('menu '):
            item = self.menuitem_from_config(cfg)
            self.add_menuitem(item.id, item)

    # buttons & encoder callbacks
    def encoder_cw_callback(self, eventtime):
        fast_rate = ((eventtime - self._last_encoder_cw_eventtime)
                     <= self._encoder_fast_rate)
        self._last_encoder_cw_eventtime = eventtime
        self.up(fast_rate)

    def encoder_ccw_callback(self, eventtime):
        fast_rate = ((eventtime - self._last_encoder_ccw_eventtime)
                     <= self._encoder_fast_rate)
        self._last_encoder_ccw_eventtime = eventtime
        self.down(fast_rate)

    def click_callback(self, eventtime, state):
        if self.click_pin:
            if state:
                self._last_click_press = eventtime
            elif self._last_click_press > 0:
                if (eventtime - self._last_click_press) < LONG_PRESS_DURATION:
                    # short click
                    self._last_click_press = 0
                    self._short_click_callback(eventtime)

    def _short_click_callback(self, eventtime):
        if self.is_running():
            self.enter()
        else:
            # lets start and populate the menu items
            self.begin(eventtime)

    def _long_click_callback(self, eventtime):
        if not self.is_running():
            # lets start and populate the menu items
            self.begin(eventtime)
        else:
            if not self.push_deck_menu():
                self.enter(True)

    def push_deck_menu(self):
        container = self.stack_peek()
        if isinstance(container, MenuDeck):
            menu = container.get_longpress_menu()
            if (isinstance(menu, MenuList)
                    and not container.is_editing()
                    and menu is not container):
                self.stack_push(menu)
                self.top_row = 0
                self.selected = 0
                return True
        return False

    def back_callback(self, eventtime):
        if self.back_pin:
            self.back()

    def up_callback(self, eventtime):
        if self.up_pin:
            self.up()

    def down_callback(self, eventtime):
        if self.down_pin:
            self.down()

    def kill_callback(self, eventtime):
        if self.kill_pin:
            # Emergency Stop
            self.printer.invoke_shutdown("Shutdown due to kill button!")
