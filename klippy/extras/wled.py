# Helper script to control WLED via JSON API
#
# Copyright (C) 2021 Richard Mitchell <richardjm+klipper@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import json, logging, Queue as queue, threading, urllib2

MAX_MCU_SIZE = 500

class WLED:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[1]
        # Read config
        self.verbose = config.get('verbose', False)
        self.url = config.get('url')
        self.on_ps = config.getint('on_ps', -1, minval=-1, maxval=65535)

        # For neopixel like behaviour
        formats = {v: v for v in ["RGB", "RGBW"]}
        self.color_order = config.getchoice("color_order", formats, "RGB")
        elem_size = len(self.color_order)
        self.chain_count = config.getint('chain_count', 0, minval=1,
                                    maxval=MAX_MCU_SIZE//elem_size)
        # Initial color
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
        self.bg_queue = queue.Queue()
        self.bg_thread = threading.Thread(target=self._send_consumer)
        self.bg_thread.start()

    def _update_color_data(self, red, green, blue, white, index=None):
        red = int(red * 255. + .5)
        blue = int(blue * 255. + .5)
        green = int(green * 255. + .5)
        white = int(white * 255. + .5)
        if self.color_order == "RGB":
            color_data = [red, green, blue]
        else:
            color_data = [red, green, blue, white]
        if index is None:
            self.color_data[:] = color_data * self.chain_count
        else:
            elem_size = len(color_data)
            self.color_data[(index-1)*elem_size:index*elem_size] = color_data

    def _wled_send(self, json):
        try:
            if self.verbose:
                logging.info("WLED: url:%s json:%s", self.url, json)
            headers = {'Content-Type':'application/json'}
            request = urllib2.Request(self.url, json, headers)
            response = urllib2.urlopen(request)
            data = response.read()
            response.close()
            if self.verbose:
                logging.info("WLED: url:%s json:%s code:%d",
                             self.url, data, response.code)
        except urllib2.HTTPError as e:
            logging.error('Failure when setting wled, %d %s', e.code, e.read())
        except urllib2.URLError as e:
            logging.error('Failure when setting wled, %s', e.reason)
        except:
            logging.error(
                'Failure when setting wled, perhaps incorrect json syntax')

    def _send_consumer(self):
        while True:
            if self.verbose:
                logging.info("WLED: url:%s waiting", self.url)
            json = self.bg_queue.get() # Block until item on queue
            if json is None:
                break
            self._wled_send(json)

    def _send_producer(self, json):
        if self.verbose:
            self.gcode.respond_info("WLED: %s %s" % (self.name, json,))
        #self._wled_send(json) # Can be used for testing on the main thread
        self.bg_queue.put_nowait(json)

    cmd_WLED_ON_help = 'Turn on WLED'
    def cmd_WLED_ON(self, gcmd):
        json = "{'on': true"
        json += ", 'ps': %d" % (gcmd.get_int('PS', self.on_ps),)
        json += "}"
        self._send_producer(json)

    cmd_WLED_OFF_help = 'Turn off WLED'
    def cmd_WLED_OFF(self, gcmd):
        self._send_producer("{'on': false}")

    cmd_SET_LED_help = 'Set the color of an LED'
    def cmd_SET_LED(self, gcmd):
        # Parse parameters
        red = gcmd.get_float('RED', 0., minval=0., maxval=1.)
        green = gcmd.get_float('GREEN', 0., minval=0., maxval=1.)
        blue = gcmd.get_float('BLUE', 0., minval=0., maxval=1.)
        white = gcmd.get_float('WHITE', 0., minval=0., maxval=1.)
        index = gcmd.get_int('INDEX', None, minval=1, maxval=self.chain_count)
        transmit = gcmd.get_int('TRANSMIT', 1)
        self._update_color_data(red, green, blue, white, index)
        if transmit:
            state = {'on': True, 'seg':{'i':[]}}
            cdata = []
            elem_size = len(self.color_order)
            for i in range(self.chain_count):
                idx = i * elem_size
                elem = []
                for p in self.color_data[idx:idx+elem_size]:
                    elem.append(p)
                cdata.append(elem)
            state['seg']['i'] = cdata
            self._send_producer(json.dumps(state))

def load_config_prefix(config):
    return WLED(config)
