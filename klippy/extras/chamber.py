#

import threading

import logging
import fan

class chamber:

	def __init__(self, config):
		#	get configs
		self.control_mode = config.get( 'control_mode' )
		self.min_temp = config.getfloat( 'min_temp', minval=10 )
		self.max_temp = config.getfloat( 'max_temp', above=self.min_temp )
		self.max_delta = config.getfloat( 'max_delta', 2.0, above=0. )
		self.danger_temp = config.getfloat( 'danger_temp', self.max_temp+5, above=self.max_temp )
		#	setup sensor
		self.printer = config.get_printer()
		self.reactor = self.printer.get_reactor()
		self.gcode = self.printer.lookup_object('gcode')
		self.sensor = self.printer.lookup_object('heater').setup_sensor(config)
		self.sensor.setup_minmax(self.min_temp, self.max_temp)
		#	cooling or heating
		if self.control_mode == 'cooling':
			self.fan = fan.PrinterFan(config, default_shutdown_speed=0.)
			self.sensor.setup_callback(self.cooling_callback)
		elif self.control_mode == 'heating':
			#	implement later
			#	must fit heaterverify workflow
			pass
		else:
			logging.error("chamber: unsupported control_mode" + self.control_mode)
			raise
		#	set dict
		self.temps = { "target": 0, "actual": 0 }
		self.gcode.register_command( 'M141', self.cmd_M141,
			desc="M141 S30 - Set chamber target to 30C")
		#	remove me
		dbg = threading.Thread( target=self.debug_console, args=() )
		#dbg.start()

	def cooling_callback(self, eventtime, temp):
		self.temps.update({'actual': temp}) #smooth?
		if self.temps['target'] == 0:
			self.fan.set_speed(eventtime+.1, 0)	#	minspeed
		elif self.temps['actual'] - self.max_delta >= self.temps['target']:
			self.fan.set_speed(eventtime+.1, 1.)	#	maxspeed
		elif self.temps['actual'] <= self.temps['target'] - self.max_delta:
			self.fan.set_speed(eventtime+.1, 0.)	#	minspeed
		elif (self.temps['actual']) > self.danger_temp:
			self.gcode.cmd_M112()

	def cmd_M141(self, params):
		new_target = float( params.get('S', -1000) )
		if self.min_temp <= new_target <= self.max_temp or new_target == 0:
			self.temps.update({'target': new_target})
			return 'Chamber temperaturetarget is set to: ' + params.get('S')
		else:
			return "Chamber target out of range. " + params.get('S',-1000)

	def get_status(self, eventtime):
		return {
			"temp": self.temps['actual'] ,
			"target": self.temps['target'] ,
			"state": 1 if self.temps['target'] > 0 else 0
		}

	def debug_console(self):
		import pdb; pdb.set_trace()


def load_config(config):
	return chamber(config)