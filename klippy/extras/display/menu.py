# -*- coding: utf-8 -*-
# Basic LCD menu support
#
# Based on the RaspberryPiLcdMenu from Alan Aufderheide, February 2013
# Copyright (C) 2018  Janar Sööt <janar.soot@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import os, logging, sys, ast, re, string


class error(Exception):
    pass


# static class for cursor
class MenuCursor:
    NONE = ' '
    SELECT = '>'
    EDIT = '*'


# Menu element baseclass
class MenuElement(object):
    def __init__(self, manager, config, namespace=''):
        self.cursor = config.get('cursor', MenuCursor.SELECT)
        self._namespace = namespace
        self._manager = manager
        self._width = self._asint(config.get('width', '0'))
        self._scroll = self._asbool(config.get('scroll', 'false'))
        self._enable = self._aslist(config.get('enable', 'true'),
                                    flatten=False)
        self._name = self._asliteral(config.get('name'))
        self.__scroll_offs = 0
        self.__scroll_diff = 0
        self.__scroll_dir = None
        self.__last_state = True
        if len(self.cursor) < 1:
            raise error("Cursor with unexpected length, expecting 1.")

    # override
    def _render(self):
        return self._name

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
    def reset_editing(self):
        pass

    def eval_enable(self):
        return self._parse_bool(self._enable)

    def init(self):
        self.__clear_scroll()

    def heartbeat(self, eventtime):
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

    def _parse_bool(self, lst):
        try:
            return any([
                all([
                    self._lookup_bool(l2) for l2 in self._words_aslist(l1)
                ]) for l1 in lst
            ])
        except Exception:
            logging.exception("Boolean parsing error")
            return False

    def _lookup_bool(self, b):
        if not self._asbool(b):
            if b[0] == '!':  # logical negation:
                return not (not not self._lookup_parameter(b[1:]))
            else:
                return not not self._lookup_parameter(b)
        return True

    def _lookup_parameter(self, literal):
        if self._isfloat(literal):
            return float(literal)
        else:
            # only 2 level dot notation
            keys = literal.rsplit('.', 1)
            name = keys[0] if keys[0:1] else None
            attr = keys[1] if keys[1:2] else None
            if isinstance(self._manager.parameters, dict):
                return (self._manager.parameters.get(name) or {}).get(attr)
            else:
                logging.error("Parameter storage is not dictionary")
        return None

    def _asliteral(self, s):
        s = str(s).strip()
        if s.startswith(('"', "'")):
            s = s[1:]
        if s.endswith(('"', "'")):
            s = s[:-1]
        return s

    def _asbool(self, s, default=False):
        if s is None:
            return default
        if isinstance(s, bool):
            return s
        s = str(s).strip()
        return s.lower() in ('y', 'yes', 't', 'true', 'on', '1')

    def _asint(self, s, default=0):
        if s is None:
            return default
        if isinstance(s, (int, float)):
            return int(s)
        s = str(s).strip()
        return int(float(s)) if self._isfloat(s) else int(default)

    def _asfloat(self, s, default=0.0):
        if s is None:
            return default
        if isinstance(s, (int, float)):
            return float(s)
        s = str(s).strip()
        return float(s) if self._isfloat(s) else float(default)

    def _lines_aslist(self, value, default=[]):
        if isinstance(value, str):
            value = filter(None, [x.strip() for x in value.splitlines()])
        try:
            return list(value)
        except Exception:
            logging.exception("Lines as list parsing error")
            return list(default)

    def _words_aslist(self, value, sep=',', default=[]):
        if isinstance(value, str):
            value = filter(None, [x.strip() for x in value.split(sep)])
        try:
            return list(value)
        except Exception:
            logging.exception("Words as list parsing error")
            return list(default)

    def _aslist(self, value, flatten=True, default=[]):
        values = self._lines_aslist(value)
        if not flatten:
            return values
        result = []
        for value in values:
            subvalues = self._words_aslist(value, sep=',')
            result.extend(subvalues)
        return result

    def _isfloat(self, value):
        try:
            float(value)
            return True
        except ValueError:
            return False

    @property
    def namespace(self):
        return self._namespace

    @namespace.setter
    def namespace(self, ns):
        self._namespace = ns


# menu container baseclass
class MenuContainer(MenuElement):
    def __init__(self, manager, config, namespace=''):
        super(MenuContainer, self).__init__(manager, config, namespace)
        self._show_back = self._asbool(config.get('show_back', 'true'))
        self._show_title = self._asbool(config.get('show_title', 'true'))
        self._allitems = []
        self._items = []
        # recursive guard
        self._parents = []

    # overload
    def _names_aslist(self):
        return []

    # overload
    def is_accepted(self, item):
        return isinstance(item, MenuElement)

    def is_readonly(self):
        return False

    def is_editing(self):
        return any([item.is_editing() for item in self._items])

    def reset_editing(self):
        for item in self._items:
            if item.is_editing():
                item.reset_editing()

    def _lookup_item(self, item):
        if isinstance(item, str):
            s = item.strip()
            if s.startswith('.'):
                s = ' '.join([self.namespace, s[1:]])
            item = self._manager.lookup_menuitem(s)
        return item

    def find_item(self, item):
        index = None
        if item in self._items:
            index = self._items.index(item)
        else:
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
            "Recursive relation of '%s' container" % (self.namespace,)

    def append_item(self, s):
        item = self._lookup_item(s)
        if item is not None:
            if not self.is_accepted(item):
                raise error("Menu item '%s'is not accepted!" % str(type(item)))
            if isinstance(item, (MenuContainer)):
                item.add_parents(self._parents)
                item.add_parents(self)
                item.assert_recursive_relation()
                item.populate_items()
            self._allitems.append(item)

    def populate_items(self):
        self._allitems = []  # empty list
        if self._show_back is True:
            name = '[..]'
            if self._show_title:
                name += ' %s' % str(self._name)
            self.append_item(MenuCommand(self._manager, {
                'name': name, 'gcode': '', 'action': 'back'}, self.namespace))
        for name in self._names_aslist():
            self.append_item(name)
        self.update_items()

    def update_items(self):
        self._items = [item for item in self._allitems if item.is_enabled()]

    def __iter__(self):
        return iter(self._items)

    def __len__(self):
        return len(self._items)

    def __getitem__(self, key):
        return self._items[key]


class MenuItem(MenuElement):
    def __init__(self, manager, config, namespace=''):
        super(MenuItem, self).__init__(manager, config, namespace)
        self.parameter = config.get('parameter', '')
        self.transform = config.get('transform', '')

    def _parse_transform(self, t):
        flist = {
            'int': int,
            'float': float,
            'bool': bool,
            'str': str,
            'abs': abs,
            'bin': bin,
            'hex': hex,
            'oct': oct
        }

        def mapper(left_min, left_max, right_min, right_max, cast_fn, index=0):
            # interpolate
            left_span = left_max - left_min
            right_span = right_max - right_min
            scale_factor = float(right_span) / float(left_span)

            def map_fn(values):
                return cast_fn(
                    right_min + (values[index] - left_min) * scale_factor
                )
            return map_fn

        def scaler(scale_factor, cast_fn, index=0):
            def scale_fn(values):
                return cast_fn(values[index] * scale_factor)
            return scale_fn

        def chooser(choices, cast_fn, index=0):
            def choose_fn(values):
                return choices[cast_fn(values[index])]
            return choose_fn

        def timerizer(key, index=0):
            time = {}

            def time_fn(values):
                try:
                    seconds = int(values[index])
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

        def functionizer(key, index=0):
            def func_fn(values):
                if key in flist and callable(flist[key]):
                    return flist[key](values[index])
                else:
                    logging.error("Unknown function: '%s'" % str(key))
                    return values[index]
            return func_fn

        fn = None
        t = str(t).strip()
        # transform: idx.func(a,b,...)
        m = re.search(r"^(\d*)(?:\.?)([\S]+)(\(.*\))$", t)
        if m is not None:
            index = int(m.group(1) or 0)
            fname = str(m.group(2)).lower()
            try:
                o = ast.literal_eval(m.group(3))
                if (fname == 'map' and isinstance(o, tuple) and len(o) == 4
                        and isinstance(o[3], (float, int))):
                    # mapper (interpolate), cast type by last parameter type
                    fn = mapper(o[0], o[1], o[2], o[3], type(o[3]), index)
                elif (fname == 'choose' and isinstance(o, tuple)
                        and len(o) == 2):
                    # boolean chooser for 2 size tuple
                    fn = chooser(o, bool, index)
                elif fname == 'choose' and isinstance(o, tuple) and len(o) > 2:
                    # int chooser for list
                    fn = chooser(o, int, index)
                elif (fname == 'choose' and isinstance(o, dict) and o.keys()
                        and isinstance(o.keys()[0], (int, float, str))):
                    # chooser, cast type by first key type
                    fn = chooser(o, type(o.keys()[0]), index)
                elif fname == 'scale' and isinstance(o, (float, int)):
                    # scaler, cast type depends from scale factor type
                    fn = scaler(o, type(o), index)
                elif fname in ('days', 'hours', 'minutes', 'seconds'):
                    fn = timerizer(fname, index)
                elif fname in flist:
                    fn = functionizer(fname, index)
                else:
                    logging.error(
                        "Unknown transform function: '%s'" % str(m.group(0)))
            except Exception:
                logging.exception("Transform parsing error")
        else:
            logging.error(
                "Invalid transform parameter: '%s'" % str(t))
        return fn

    def _transform_aslist(self):
        return list(filter(None, (
            self._parse_transform(t) for t in self._aslist(
                self.transform, flatten=False)
        )))

    def _parameter_aslist(self):
        lst = []
        for p in self._words_aslist(self.parameter):
            lst.append(self._lookup_parameter(p))
            if lst[-1] is None:
                logging.error("Parameter '%s' not found" % str(p))
        return list(lst)

    def _prepare_values(self, value=None):
        values = []
        for i, v in enumerate(self._parameter_aslist()):
            values += [value if i == 0 and value is not None else v]
        if values:
            try:
                values += [t(list(values)) for t in self._transform_aslist()]
            except Exception:
                logging.exception("Transformation execution failed")
        return tuple(values)

    def _get_formatted(self, literal, val=None):
        values = self._prepare_values(val)
        if isinstance(literal, str) and len(values) > 0:
            try:
                literal = literal.format(*values)
            except Exception:
                logging.exception("Literal formatting failed")
        return literal

    def _render(self):
        return self._get_formatted(self._name)


class MenuCommand(MenuItem):
    def __init__(self, manager, config, namespace=''):
        super(MenuCommand, self).__init__(manager, config, namespace)
        self._gcode = config.get('gcode', '')
        self._action = config.get('action', None)
        if self._action is None and not self._gcode:
            raise error("Missing or empty 'gcode' option")

    def is_readonly(self):
        return False

    def get_gcode(self):
        return self._get_formatted(self._gcode)

    def __call__(self):
        if self._action is not None:
            try:
                fmt = self._get_formatted(self._action)
                args = fmt.split()
                self._manager.run_action(args[0], *args[1:])
            except Exception:
                logging.exception("Action formatting failed")


class MenuInput(MenuCommand):
    def __init__(self, manager, config, namespace=''):
        super(MenuInput, self).__init__(manager, config, namespace)
        self._reverse = self._asbool(config.get('reverse', 'false'))
        self._realtime = self._asbool(config.get('realtime', 'false'))
        self._readonly = self._aslist(
            config.get('readonly', 'false'), flatten=False)
        self._input_value = None
        self.__last_value = None
        self._input_min = config.getfloat('input_min', sys.float_info.min)
        self._input_max = config.getfloat('input_max', sys.float_info.max)
        self._input_step = config.getfloat('input_step', above=0.)
        self._input_step2 = config.getfloat('input_step2', 0, minval=0.)
        self._longpress_gcode = config.get('longpress_gcode', '')

    def is_scrollable(self):
        return False

    def is_readonly(self):
        return self._parse_bool(self._readonly)

    def _render(self):
        return self._get_formatted(self._name, self._input_value)

    def get_gcode(self):
        return self._get_formatted(self._gcode, self._input_value)

    def get_longpress_gcode(self):
        return self._get_formatted(self._longpress_gcode, self._input_value)

    def is_editing(self):
        return self._input_value is not None

    def reset_editing(self):
        self.reset_value()

    def _onchange(self):
        self._manager.queue_gcode(self.get_gcode())

    def init_value(self):
        self._input_value = None
        self.__last_value = None
        if not self.is_readonly():
            args = self._prepare_values()
            if len(args) > 0 and self._isfloat(args[0]):
                self._input_value = float(args[0])
                if self._realtime:
                    self._onchange()
            else:
                logging.error("Cannot init input value")

    def reset_value(self):
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
            self._onchange()

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
            self._onchange()


class MenuGroup(MenuContainer):
    def __init__(self, manager, config, namespace='', sep=','):
        super(MenuGroup, self).__init__(manager, config, namespace)
        self._sep = sep
        self._show_back = False
        self.selected = None
        self.use_cursor = self._asbool(config.get('use_cursor', 'false'))
        self.items = config.get('items', '')

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
        return self._words_aslist(self.items, sep=self._sep)

    def init(self):
        super(MenuGroup, self).init()
        for item in self._items:
            item.init()

    def _render_item(self, item, selected=False, scroll=False):
        name = "%s" % str(item.render(scroll))
        if selected and not self.is_editing():
            if self.use_cursor:
                name = (item.cursor if isinstance(item, MenuElement)
                        else MenuCursor.SELECT) + name
            else:
                name = (name if self._manager.blink_slow_state
                        else ' '*len(name))
        elif selected and self.is_editing():
            if self.use_cursor:
                name = MenuCursor.EDIT + name
            else:
                name = (name if self._manager.blink_fast_state
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

    def reset_editing(self):
        return self._call_selected('reset_editing')

    def is_editing(self):
        return self._call_selected('is_editing')

    def inc_value(self, fast_rate=False):
        self._call_selected('inc_value', fast_rate)

    def dec_value(self, fast_rate=False):
        self._call_selected('dec_value', fast_rate)

    def selected_item(self):
        return self._call_selected()

    def find_next_item(self):
        if self.selected is None:
            self.selected = 0
        elif self.selected < len(self) - 1:
            self.selected += 1
        else:
            self.selected = None
        # skip readonly
        while (self.selected is not None
                and self.selected < len(self)
                and self._call_selected('is_readonly')):
            if self.selected < len(self) - 1:
                self.selected = (self.selected + 1)
            else:
                self.selected = None
        return self.selected

    def find_prev_item(self):
        if self.selected is None:
            self.selected = len(self) - 1
        elif self.selected > 0:
            self.selected -= 1
        else:
            self.selected = None
        # skip readonly
        while (self.selected is not None
                and self.selected >= 0
                and self._call_selected('is_readonly')):
            self.selected = (self.selected - 1) if self.selected > 0 else None
        return self.selected


class MenuItemGroup(MenuGroup):
    def __init__(self, manager, config, namespace='', sep='|'):
        super(MenuItemGroup, self).__init__(manager, config, namespace, sep)

    def is_readonly(self):
        return True

    def is_accepted(self, item):
        return type(item) is MenuItem


class MenuCycler(MenuGroup):
    def __init__(self, manager, config, namespace='', sep=','):
        super(MenuCycler, self).__init__(manager, config, namespace, sep)
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
            item = MenuItemGroup(self._manager, {
                'name': ' '.join([self._name, 'ItemGroup']),
                'items': item
            }, self.namespace, '|')
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
    def __init__(self, manager, config, namespace=''):
        super(MenuList, self).__init__(manager, config, namespace)
        self._enter_gcode = config.get('enter_gcode', None)
        self._leave_gcode = config.get('leave_gcode', None)
        self.items = config.get('items', '')

    def is_accepted(self, item):
        return (super(MenuList, self).is_accepted(item)
                and type(item) is not MenuCard)

    def _names_aslist(self):
        return self._lines_aslist(self.items)

    def _lookup_item(self, item):
        if isinstance(item, str) and ',' in item:
            item = MenuGroup(self._manager, {
                'name': ' '.join([self._name, 'Group']),
                'items': item
            }, self.namespace, ',')
        return super(MenuList, self)._lookup_item(item)

    def update_items(self):
        super(MenuList, self).update_items()
        for item in self._items:
            if isinstance(item, MenuGroup) and not item.is_editing():
                item.update_items()

    def get_enter_gcode(self):
        return self._enter_gcode

    def get_leave_gcode(self):
        return self._leave_gcode


class MenuVSDCard(MenuList):
    def __init__(self, manager, config, namespace=''):
        super(MenuVSDCard, self).__init__(manager, config, namespace)

    def _populate_files(self):
        sdcard = self._manager.objs.get('virtual_sdcard')
        if sdcard is not None:
            files = sdcard.get_file_list()
            for fname, fsize in files:
                gcode = [
                    'M23 /%s' % str(fname)
                ]
                self.append_item(MenuCommand(self._manager, {
                    'name': '%s' % str(fname),
                    'cursor': '+',
                    'gcode': "\n".join(gcode),
                    'scroll': True,
                    # mind the cursor size in width
                    'width': (self._manager.cols-1)
                }))

    def populate_items(self):
        super(MenuVSDCard, self).populate_items()
        self._populate_files()


class MenuCard(MenuGroup):
    def __init__(self, manager, config, namespace=''):
        super(MenuCard, self).__init__(manager, config, namespace)
        self.content = config.get('content')
        self._allow_without_selection = self._asbool(
            config.get('allow_without_selection', 'true'))
        if not self.items:
            self.content = self._parse_content_items(self.content)

    def _parse_content_items(self, content):
        formatter = string.Formatter()
        out = ""
        items = []

        try:
            parsed_content = list(formatter.parse(content))
        except Exception:
            logging.exception("Card content parsing error")

        for part in parsed_content:
            # (literal_text, field_name, format_spec, conversion)
            out += part[0]
            if part[1]:
                out += "{%s%s%s}" % (
                    len(items),
                    ("!" + part[3]) if part[3] else '',
                    (":" + part[2]) if part[2] else '',
                )
                items.append(str(part[1]))

        self.items = "\n".join(items)
        return out

    def _names_aslist(self):
        return self._lines_aslist(self.items)

    def _content_aslist(self):
        return filter(None, [
            self._asliteral(s) for s in self._lines_aslist(self.content)
        ])

    def update_items(self):
        self._items = self._allitems[:]
        for item in self._items:
            if isinstance(item, MenuGroup) and not item.is_editing():
                item.update_items()

    def _lookup_item(self, item):
        if isinstance(item, str) and ',' in item:
            item = MenuCycler(self._manager, {
                'name': ' '.join([self._name, 'Cycler']),
                'items': item
            }, self.namespace, ',')
        return super(MenuCard, self)._lookup_item(item)

    def render_content(self, eventtime):
        if self.selected is not None:
            self.selected = (
                (self.selected % len(self)) if len(self) > 0 else None)
        if self._allow_without_selection is False and self.selected is None:
            self.selected = 0 if len(self) > 0 else None

        items = []
        for i, item in enumerate(self):
            name = ''
            if item.is_enabled():
                item.heartbeat(eventtime)
                name = self._render_item(item, (i == self.selected), True)
            items.append(name)
        lines = []
        for line in self._content_aslist():
            try:
                lines.append(str(line).format(*items))
            except Exception:
                logging.exception('Card rendering error')
        return lines

    def _render(self):
        return self._name


class MenuDeck(MenuList):
    def __init__(self, manager, config, namespace=''):
        super(MenuDeck, self).__init__(manager, config, namespace)
        self._menu = config.get('longpress_menu', None)
        self.menu = None
        self._show_back = False
        self._show_title = False
        if not self.items:
            card = MenuCard(self._manager, {
                'name': ' '.join([self._name, 'Card']),
                'use_cursor': config.get('use_cursor', 'false'),
                'allow_without_selection': config.get(
                    'allow_without_selection', 'true'),
                'content': config.get('content')
            }, self.namespace)
            name = " ".join(
                config.get_name().split()[1:]) + "__singlecarddeck__"
            self._manager.add_menuitem(name, card)
            self.items = name

    def _populate_menu(self):
        self.menu = None
        if self._menu is not None:
            menu = self._manager.lookup_menuitem(self._menu)
            if isinstance(menu, MenuContainer):
                menu.assert_recursive_relation(self._parents)
                menu.populate_items()
                self.menu = menu

    def populate_items(self):
        super(MenuDeck, self).populate_items()
        self._populate_menu()

    def _names_aslist(self):
        return self._aslist(self.items)

    def is_accepted(self, item):
        return type(item) is MenuCard

    def _render(self):
        return self._name


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
    def __init__(self, config, lcd_chip):
        self.running = False
        self.menuitems = {}
        self.menustack = []
        self._autorun = False
        self.top_row = 0
        self.selected = 0
        self.blink_fast_state = True
        self.blink_slow_state = True
        self.blink_fast_idx = 0
        self.blink_slow_idx = 0
        self.timeout_idx = 0
        self.lcd_chip = lcd_chip
        self.printer = config.get_printer()
        self.pconfig = self.printer.lookup_object('configfile')
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode_queue = []
        self.parameters = {}
        self.objs = {}
        self.root = None
        self._root = config.get('menu_root', '__main')
        self.cols, self.rows = lcd_chip.get_dimensions()
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

        # Add MENU commands
        self.gcode.register_mux_command("MENU", "DO", 'dump', self.cmd_DO_DUMP,
                                        desc=self.cmd_DO_help)

        # Load local config file in same directory as current module
        self.load_config(os.path.dirname(__file__), 'menu.cfg')
        # Load items from main config
        self.load_menuitems(config)
        # Load menu root
        self.load_root()

    def handle_ready(self):
        # Load all available printer objects
        for cfg_name, obj in self.printer.lookup_objects():
            name = ".".join(str(cfg_name).split())
            self.objs[name] = obj
            logging.debug("Load module '%s' -> %s" % (
                str(name), str(obj.__class__)))
        # start timer
        reactor = self.printer.get_reactor()
        reactor.register_timer(self.timer_event, reactor.NOW)

    def timer_event(self, eventtime):
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
                and not self._timeout_autorun_root()):
            if self.timer >= self.timeout:
                self.exit()
            self.timer += 1
        else:
            self.timer = 0

    def _timeout_autorun_root(self):
        return (self._autorun is True and self.root is not None
                and self.stack_peek() is self.root and self.selected == 0)

    def restart_root(self, root=None, force_exit=True):
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

    def after(self, timeout, callback, *args):
        """Helper method for reactor.register_callback.
        The callback will be executed once after given timeout (sec)."""
        def callit(eventtime):
            callback(eventtime, *args)
        reactor = self.printer.get_reactor()
        starttime = reactor.monotonic() + max(0., float(timeout))
        reactor.register_callback(callit, starttime)

    def is_running(self):
        return self.running

    def begin(self, eventtime):
        self.menustack = []
        self.top_row = 0
        self.selected = 0
        self.timer = 0
        if isinstance(self.root, MenuContainer):
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
            'isRunning': self.running,
            'is2004': (self.rows == 4 and self.cols == 20),
            'is2002': (self.rows == 2 and self.cols == 20),
            'is1604': (self.rows == 4 and self.cols == 16),
            'is1602': (self.rows == 2 and self.cols == 16),
            'is20xx': (self.cols == 20),
            'is16xx': (self.cols == 16)
        }

    def update_parameters(self, eventtime):
        self.parameters = {}
        objs = dict(self.objs)
        # getting info this way is more like hack
        # all modules should have special reporting method (maybe get_status)
        # for available parameters
        # Only 2 level dot notation
        for name in objs.keys():
            try:
                if objs[name] is not None:
                    class_name = str(objs[name].__class__.__name__)
                    get_status = getattr(objs[name], "get_status", None)
                    if callable(get_status):
                        self.parameters[name] = get_status(eventtime)
                    else:
                        self.parameters[name] = {}

                    self.parameters[name].update({'is_enabled': True})
                    # get additional info
                    if class_name == 'ToolHead':
                        pos = objs[name].get_position()
                        self.parameters[name].update({
                            'xpos': pos[0],
                            'ypos': pos[1],
                            'zpos': pos[2],
                            'epos': pos[3]
                        })
                        self.parameters[name].update({
                            'is_printing': (
                                self.parameters[name]['status'] == "Printing"),
                            'is_ready': (
                                self.parameters[name]['status'] == "Ready"),
                            'is_idle': (
                                self.parameters[name]['status'] == "Idle")
                        })
                else:
                    self.parameters[name] = {'is_enabled': False}
            except Exception:
                logging.exception("Parameter '%s' update error" % str(name))

    def stack_push(self, container):
        if not isinstance(container, MenuContainer):
            raise error("Wrong type, expected MenuContainer")
        top = self.stack_peek()
        if top is not None:
            self.queue_gcode(top.get_leave_gcode())
        self.queue_gcode(container.get_enter_gcode())
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
                self.queue_gcode(container.get_leave_gcode())
                self.queue_gcode(top.get_enter_gcode())
            else:
                self.queue_gcode(container.get_leave_gcode())
        return container

    def stack_size(self):
        return len(self.menustack)

    def stack_peek(self, lvl=0):
        container = None
        if self.stack_size() > lvl:
            container = self.menustack[self.stack_size() - lvl - 1]
        return container

    def _unescape_cchars(self, text):
        def fixup(m):
            text = str(m.group(0))
            if text[:2] == "\\x":
                try:
                    return "%c" % (int(text[2:], 16),)
                except ValueError:
                    logging.exception('Custom character unescape error')
            else:
                return text
        return re.sub(r'\\x[0-9a-f]{2}', fixup, str(text), flags=re.IGNORECASE)

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
                lines = container[self.selected].render_content(eventtime)
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
                            elif isinstance(current, MenuElement):
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
                self.lcd_chip.write_text(0, y, self._unescape_cchars(line))
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
                if isinstance(container[self.selected], MenuElement):
                    container[self.selected].init()
                # wind up group last item or init item
                if isinstance(container[self.selected], MenuGroup):
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
                if isinstance(container[self.selected], MenuElement):
                    container[self.selected].init()
                # wind up group first item
                if isinstance(container[self.selected], MenuGroup):
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
                if isinstance(parent[self.selected], MenuElement):
                    parent[self.selected].init()
                # wind up group first item or init item
                if isinstance(parent[self.selected], MenuGroup):
                    parent[self.selected].find_next_item()
            else:
                self.stack_pop()
                self.running = False

    def select(self, long_press=False):
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
                        self.queue_gcode(current.get_gcode())
                        self.queue_gcode(current.get_longpress_gcode())
                    else:
                        self.queue_gcode(current.get_gcode())
                        current.reset_value()
                else:
                    current.init_value()
            elif isinstance(current, MenuCommand):
                current()
                self.queue_gcode(current.get_gcode())

    def exit(self, force=False):
        container = self.stack_peek()
        if self.running and isinstance(container, MenuContainer):
            current = container[self.selected]
            if (not force and isinstance(current, (MenuInput, MenuGroup))
                    and current.is_editing()):
                return
            self.queue_gcode(container.get_leave_gcode())
            self.running = False

    def run_action(self, action, *args):
        try:
            action = str(action).strip().lower()
            if action == 'back':
                self.back()
            elif action == 'exit':
                self.exit()
            elif action == 'respond':
                self.gcode.respond_info("{}".format(' '.join(map(str, args))),
                                        log=False)
            elif action == 'event' and len(args) > 0:
                if len(str(args[0])) > 0:
                    self.printer.send_event(
                        "menu:action:" + str(args[0]), *args[1:])
                else:
                    logging.error("Malformed event call: {} {}".format(
                        action, ' '.join(map(str, args))))
            else:
                logging.error("Unknown action %s" % (action))
        except Exception:
            logging.exception("Malformed action call")

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

    def add_menuitem(self, name, menu):
        if name in self.menuitems:
            logging.info(
                "Declaration of '%s' hides "
                "previous menuitem declaration" % (name,))
        self.menuitems[name] = menu

    def lookup_menuitem(self, name):
        if name is None:
            return None
        if name not in self.menuitems:
            raise self.printer.config_error(
                "Unknown menuitem '%s'" % (name,))
        return self.menuitems[name]

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
            name = " ".join(cfg.get_name().split()[1:])
            item = cfg.getchoice('type', menu_items)(self, cfg, name)
            self.add_menuitem(name, item)

    cmd_DO_help = "Menu do things"

    def cmd_DO_DUMP(self, params):
        for key1 in self.parameters:
            if type(self.parameters[key1]) == dict:
                for key2 in self.parameters[key1]:
                    msg = "{0}.{1} = {2}".format(
                        key1, key2,
                        self.parameters[key1].get(key2)
                    )
                    self.gcode.respond_info(msg)
            else:
                msg = "{0} = {1}".format(key1, self.parameters.get(key1))
                self.gcode.respond_info(msg)

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
            self.select()
        else:
            # lets start and populate the menu items
            self.begin(eventtime)

    def _long_click_callback(self, eventtime):
        if not self.is_running():
            # lets start and populate the menu items
            self.begin(eventtime)
        else:
            container = self.stack_peek()
            if isinstance(container, MenuDeck):
                menu = container.menu
                if (isinstance(menu, MenuList)
                        and not container.is_editing()
                        and menu is not container):
                    self.stack_push(menu)
                    self.top_row = 0
                    self.selected = 0
                    return
            if container.is_editing():
                self.select(True)

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
