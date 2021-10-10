# Helper script to control WLED via JSON API
#
# Copyright (C) 2021 Richard Mitchell <richardjm+klipper@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import json, logging, threading

# Python2 or Python3 imports
try:
    import urlparse
except:
    import urllib.parse as urlparse

try:
    from queue import Queue
except ImportError:
    from Queue import Queue

try:
    import httplib
except ImportError:
    import http.client

MAX_MCU_SIZE = 500

class WLED:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[1]
        # Read config
        self.debug = config.get('debug', False)
        self.url = urlparse.urlparse(config.get('url'))
        self.port = None
        self.usehttps = self.url.scheme == 'https'
        if self.url.netloc.find(':') == -1:
            if self.usehttps:
                self.port = 443
            else:
                self.port = 80
        self.timeout = config.getfloat('timeout', 2., minval=0.1, maxval=10.)
        self.on_ps = config.getint('on_ps', -1, minval=-1, maxval=65535)

        # For neopixel like behaviour
        formats = {v: v for v in ['RGB', 'RGBW']}
        self.color_order = config.getchoice('color_order', formats, 'RGB')
        elem_size = len(self.color_order)
        self.chain_count = config.getint('chain_count', 0, minval=1,
                                    maxval=MAX_MCU_SIZE//elem_size)
        # Initial color
        self.send_on_color_data = True
        self.send_full_color_data = True
        self.color_data = bytearray(self.chain_count * elem_size)
        red = config.getfloat('initial_RED', 0., minval=0., maxval=1.)
        green = config.getfloat('initial_GREEN', 0., minval=0., maxval=1.)
        blue = config.getfloat('initial_BLUE', 0., minval=0., maxval=1.)
        white = 0
        if elem_size == 4:
            white = config.getfloat('initial_WHITE', 0., minval=0., maxval=1.)
        self._update_color_data(red, green, blue, white)

        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_mux_command('SET_LED', 'LED', self.name,
                            self.cmd_SET_LED, desc=self.cmd_SET_LED_help)
        self.gcode.register_mux_command('WLED_ON', 'WLED', self.name,
                                    self.cmd_WLED_ON,
                                    desc=self.cmd_WLED_ON_help)
        self.gcode.register_mux_command('WLED_OFF', 'WLED', self.name,
                            self.cmd_WLED_OFF, desc=self.cmd_WLED_OFF_help)

        # Consumer of producer/consumer
        self.bg_queue = Queue.Queue()
        self.bg_thread = threading.Thread(target=self._send_consumer)
        self.bg_thread.start()

    def _update_color_data(self, red, green, blue, white, index=None):
        red = int(red * 255. + .5)
        blue = int(blue * 255. + .5)
        green = int(green * 255. + .5)
        white = int(white * 255. + .5)
        if self.color_order == 'RGB':
            color_data = [red, green, blue]
        else:
            color_data = [red, green, blue, white]
        if index is None:
            self.color_data[:] = color_data * self.chain_count
        else:
            elem_size = len(color_data)
            self.color_data[(index-1)*elem_size:index*elem_size] = color_data
        # if self.debug:
        #     logging.info('WLED: color_data:%s self.color_data:%s', color_data,
        #                 self.color_data)

    def _wled_send(self, state):
        try:
            state = json.dumps(state)

            if self.debug:
                logging.info('WLED: host:%s json:%s', self.url.netloc, state)

            if self.usehttps:
                conn = httplib.HTTPSConnection(self.url.netloc, self.port,
                        timeout=self.timeout)
            else:
                conn = httplib.HTTPConnection(self.url.netloc, self.port,
                        timeout=self.timeout)
            conn.connect()
            request = conn.putrequest('POST', self.url.path)
            conn.putheader('Content-Type', 'application/json')
            conn.putheader('Content-Length', len(state))
            conn.endheaders()
            conn.send(state)

            response = conn.getresponse()

            # Return information not used currently
            data = 'Not read'
            #data = response.read()
            conn.close()
            if self.debug:
                logging.info('WLED: host:%s response:%s status:%d',
                             self.url.netloc, data, response.status)
        except Exception as e:
           logging.error('Failure when setting wled: %s', e)

    def _send_consumer(self):
        while True:
            if self.debug:
                logging.info('WLED: host:%s waiting', self.url.netloc)
            state = self.bg_queue.get() # Block until item on queue
            if state is None:
                break
            self._wled_send(state)

    def _send_producer(self, state):
        if self.debug:
            self.gcode.respond_info('WLED: %s %s' % (self.name, state,))
        # Can be used for testing on the main thread
        #self._wled_send(state)
        self.bg_queue.put_nowait(state)

    cmd_WLED_ON_help = 'Turn on WLED with preset'
    def cmd_WLED_ON(self, gcmd):
        self.send_full_color_data = True
        state = {'on': True, 'ps': gcmd.get_int('PS', self.on_ps) }
        self._send_producer(state)

    cmd_WLED_OFF_help = 'Turn off WLED'
    def cmd_WLED_OFF(self, gcmd):
        self.send_on_color_data = True
        self._send_producer({'on': False })

    cmd_SET_LED_help = 'Set the color of an LED'
    def cmd_SET_LED(self, gcmd):
        # Parse parameters
        red = gcmd.get_float('RED', 0., minval=0., maxval=1.)
        green = gcmd.get_float('GREEN', 0., minval=0., maxval=1.)
        blue = gcmd.get_float('BLUE', 0., minval=0., maxval=1.)
        white = gcmd.get_float('WHITE', 0., minval=0., maxval=1.)
        index = gcmd.get_int('INDEX', None, minval=1, maxval=self.chain_count)
        transmit = gcmd.get_int('TRANSMIT', 1)
        if self.debug:
            self.gcode.respond_info(
            'WLED: %s R=%f G=%f B=%f W=%f INDEX=%s TRANSMIT=%s' %
            (self.name, red, green, blue, white, index, transmit,))
        self._update_color_data(red, green, blue, white, index)
        if transmit:
            elem_size = len(self.color_order)
            if self.send_on_color_data:
                # Without a separate On call individual led control doesn't
                # turn the led strip back on
                self.send_on_color_data = False
                self._send_producer({'on': True})
            if index is None:
                # All pixels same color only send range command
                elem = []
                for p in self.color_data[0:elem_size]:
                    elem.append(p)
                self.send_full_color_data = False
                self._send_producer({'seg':{'i':[0,self.chain_count-1, elem]}})
            elif self.send_full_color_data:
                # Send a full set of color data (e.g. previous preset)
                state = {'seg':{'i':[]}}
                cdata = []
                for i in range(self.chain_count):
                    idx = i * elem_size
                    elem = []
                    for p in self.color_data[idx:idx+elem_size]:
                        elem.append(p)
                    cdata.append(elem)
                state['seg']['i'] = cdata
                self.send_full_color_data = False
                self._send_producer(state)
            else:
                # Only one pixel has changed so send just that one
                elem = []
                for p in self.color_data[(index - 1) * elem_size
                                         :(index - 1) * elem_size + elem_size]:
                    elem.append(p)
                self._send_producer({'seg':{'i':[index, elem]}})
        elif index is not None:
            self.send_full_color_data = True

def load_config_prefix(config):
    return WLED(config)
