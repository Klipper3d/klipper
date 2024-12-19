import logging, math, json, collections
from enum import Enum
from . import probe

class ConcentricityToleranceCompansationError(Exception):
    pass

# PEP 485 isclose()
def isclose(a, b, rel_tol=1e-09, abs_tol=0.0):
    return abs(a-b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)

# Linear interpolation between two values
def lerp(t, v0, v1):
    return (1. - t) * v0 + t * v1

class CompensationType(Enum):
    BM = "BedMesh"
    CTC = "ConcentricityToleranceCompansation"


class MoveTransformer:
    
    def __init__(self, config):
        self.config = config
        self.bed_mesh_compensation = None
        self.concentricity_tolerance_compensation = None
        self.last_position = [0., 0., 0., 0., 0.]
        
        #self.has_concentricy_tolerance_compensation = config.getboolean('has_concentricy_tolerance_compensation', False)
        #self.has_bed_mesh = config.getboolean('has_bed_mesh', False)
        
        self.printer = config.get_printer()
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        
        self.compensations = {CompensationType.BM : self.bed_mesh_compensation, 
                              CompensationType.CTC : self.concentricity_tolerance_compensation}
        
        
    def initialize(self):
        if self.compensations[CompensationType.BM] != None and self.compensations[CompensationType.CTC] != None:
            
            # Register transformation/ compensation when availabled
            gcode_move = self.printer.load_object(self.config, 'gcode_move')
            gcode_move.set_move_transform(self)
            
            # cache the current position before a transform takes place
            gcode_move = self.printer.lookup_object('gcode_move')
            gcode_move.reset_last_position()
            
        elif self.compensations[CompensationType.BM] != None:
            # Register transformation/ compensation when availabled
            gcode_move = self.printer.load_object(self.bed_mesh_compensation.config, 'gcode_move')
            gcode_move.set_move_transform(self.bed_mesh_compensation)
            
        elif self.compensations[CompensationType.CTC] != None:
            # Register transformation/ compensation when availabled
            gcode_move = self.printer.load_object(self.concentricity_tolerance_compensation.config, 'gcode_move')
            gcode_move.set_move_transform(self.bed_mesh_compensation)
            
    def handle_connect(self):
       self.toolhead = self.printer.lookup_object('toolhead')
        
    def set_bed_mesh_compensation(self, obj, config):
        self.compensations[CompensationType.BM] = obj
        self.bed_mesh_compensation = obj
        self.config = config
        self.initialize()
        
    def set_concentricity_tolerance_compensation(self, obj, config):
        self.compensations[CompensationType.CTC] = obj
        self.concentricity_tolerance_compensation = obj
        self.config = config
        self.initialize()
        
    def handle_connect(self):
        for key, obj in self.compensations.items():
            if obj != None:
                obj.handle_connect()

    def get_status(self, eventtime=None):
        for key, obj in self.compensations.items():
            if obj != None:
                obj.handle_connect()
                
    def get_position(self):      

        self.last_position[:] = self.toolhead.get_position()

            
        position_BM = self.compensations[CompensationType.BM].get_position_multiple_compensations(self.last_position)
        position_CTC = self.compensations[CompensationType.CTC].get_position_multiple_compensations(position_BM)
        
        self.last_position = position_CTC
        
        return list(self.last_position)
    
    
    def move(self, newpos, speed):
        move_list_bm =  self.compensations[CompensationType.BM].get_position_multiple_compensations(newpos,self.last_position)
        
        for move in move_list_bm:
            self.compensations[CompensationType.CTC].move(move,speed)
            

        
def load_config(config):
    return MoveTransformer(config)
