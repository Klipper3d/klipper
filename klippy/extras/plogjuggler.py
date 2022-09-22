# Module for streaming data to plotjuggler
#
# Copyright (C) 2022  Konstantin Vogel <konstantin.vogel@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
import json
import websocket # pip install websocket-client
import time
import queue
from threading import Thread

class Plotjuggler:
    def __init__(self, config):
        self.host_adress = config.get('host_adress')
        self.data_queue = queue.Queue()
        self.reactor = config.get_reactor()
        self.ws = None
        self.reconnect_time = 0
        self.reactor.register_event_handler("klippy:disconnect",
            self.handle_disconnect)
        Thread(target=self.run_server, args=[]).start()

    def run_server(self):
        while self.reactor._process:
            data = json.dumps(self.data_queue.get())
            try:
                self.ws.send(data)
            except:
                now = time.time()
                if now > self.reconnect_time:
                    self.reconnect_time = now + 5
                    self.ws = websocket.WebSocket(skip_utf8_validation=True)
                    try:
                        self.ws.connect("ws://" + self.host_adress)
                    except:
                        self.ws = None

    def send_data(self, timestamp, name, data):
        self.data_queue.put_nowait({'timestamp': timestamp, name: data})

    def handle_disconnect(self):
        self.data_queue.put_nowait({}) # Send dummy event to trigger the loop

def load_config(config):
    return Plotjuggler(config)
