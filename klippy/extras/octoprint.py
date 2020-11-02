# Octoprint REST API support
#
# Copyright (C) 2019  Stanislav Kljuhhin <stanislav.kljuhhin@me.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import requests, logging


DEFAULT_HOST = 'http://localhost:5000'
GET_FILES = '/api/files'

class Octoprint:
    def __init__(self, config):
        self._host = config.get('host', DEFAULT_HOST)
        self._headers = {'X-Api-Key': config.get('api_key')}
        self._gcode = config.get_printer().lookup_object('gcode')
        self._gcode.register_command('OCTOPRINT', self.cmd_OCTOPRINT)

    def list_files(self):
        try:
            r = requests.get(self._host + GET_FILES, headers=self._headers)
            r.raise_for_status()
        except requests.exceptions.RequestException:
            logging.exception('Octoprint: '
                              'exception when requesting the file list')
            return
        for file in r.json().get('files', []):
            yield file['name'], file['refs']['resource']

    def _print_file(self, resource):
        json = {'command': 'select', 'print': True}
        try:
            r = requests.post(resource, headers=self._headers, json=json)
            r.raise_for_status()
        except requests.exceptions.RequestException:
            logging.exception('Octoprint: exception when starting the print')

    def cmd_OCTOPRINT(self, params):
        print_file = params.get('PRINT_FILE')
        if print_file is None:
            self._gcode.respond_info('Required parameter PRINT_FILE missing.')
            return
        self._print_file(print_file)
        self._gcode.respond_info('Printing %s' % print_file)


def load_config(config):
    return Octoprint(config)
