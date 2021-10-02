# Helper script to control WLED via JSON API
#
# Copyright (C) 2021 Richard Mitchell <richardjm+klipper@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, Queue as queue, threading, urllib2
    
class WLED:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[1]
        # Read config
        self.verbose = config.get('verbose', False)
        self.url = config.get('url')
        self.on_ps = config.getint('on_ps', -1, minval=-1, maxval=65535)
        # Register commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_mux_command('WLED_JSON', 'WLED', self.name,
                            self.cmd_WLED_JSON, desc=self.cmd_WLED_JSON_help)
        self.gcode.register_mux_command('WLED_ON', 'WLED', self.name,
                                    self.cmd_WLED_ON, desc=self.cmd_WLED_ON_help)
        self.gcode.register_mux_command('WLED_OFF', 'WLED', self.name,
                            self.cmd_WLED_OFF, desc=self.cmd_WLED_OFF_help)
        
        # Consumer of producer/consumer
        self.bg_queue = queue.Queue()
        self.bg_thread = threading.Thread(target=self._send_consumer)
        self.bg_thread.start()
        
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
                logging.info("WLED: url:%s json:%s code:%d", self.url, data, response.code)
        except urllib2.HTTPError as e:
            logging.error('Failure when setting wled, %d %s', e.code, e.read())
        except urllib2.URLError as e:
            logging.error('Failure when setting wled, %s', e.reason)
        except:
            logging.error('Failure when setting wled, perhaps incorrect json syntax')
            
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
        
    cmd_WLED_JSON_help = 'Send json data to WLED'
    def cmd_WLED_JSON(self, gcmd):
        json = gcmd.get('JSON')
        self._send_producer(json)

    cmd_WLED_ON_help = 'Turn on WLED'
    def cmd_WLED_ON(self, gcmd):
        json = "{'on': true"
        json += ", 'ps': %d" % (gcmd.get_int('PS', self.on_ps),)
        json += "}"
        self._send_producer(json)

    cmd_WLED_OFF_help = 'Turn off WLED'
    def cmd_WLED_OFF(self, gcmd):
        self._send_producer("{'on': false}")

def load_config_prefix(config):
    return WLED(config)
