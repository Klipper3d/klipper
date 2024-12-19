import logging, math, json, collections
from . import probe

class ConcentricityToleranceCompansationError(Exception):
    pass

# PEP 485 isclose()
def isclose(a, b, rel_tol=1e-09, abs_tol=0.0):
    return abs(a-b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)

# Linear interpolation between two values
def lerp(t, v0, v1):
    return (1. - t) * v0 + t * v1


class ConcentricityToleranceCompansation:
    
    def __init__(self, config):
        self.config = config
        self.printer = config.get_printer()
        self.last_position = [0., 0., 0., 0., 0.]
        
        self.deflection_angle = config.getfloat('deflection_angle', 0.)
        self.deflection_radius = config.getfloat('deflection_radius', 0.)
            
        self.x_offset = 0.
        self.y_offset = 0.
        self.gcode = self.printer.lookup_object('gcode')
        self.splitter = MoveSplitter(config, self.gcode, self.deflection_angle, self.deflection_radius)
        
        # register gcodes
        self.gcode.register_command(
            'CALIBRATE_DEFLECTION_ANGLE', self.cmd_CALIBRATE_DEFLECTION_ANGLE,
            desc=self.cmd_CALIBRATE_DEFLECTION_ANGLE_help)
        self.gcode.register_command(
            'CALIBRATE_DEFLECTION_RADIUS', self.cmd_CALIBRATE_DEFLECTION_RADIUS,
            desc=self.cmd_CALIBRATE_DEFLECTION_RADIUS_help)
        
        # Register transform
        gcode_move = self.printer.lookup_object('gcode_move')
        gcode_move.add_move_transformer(self)
        
    def calc_xy_adj(self, a_pos):
        calc_deflection_angle = math.radians(a_pos + self.deflection_angle)
        
        x_adj = self.deflection_radius * math.sin(calc_deflection_angle)
        y_adj = self.deflection_radius * math.cos(calc_deflection_angle)
        
        return x_adj, y_adj    
        
        
    def get_position(self, pos : list):
        # return current position minus the current z-adjustment
        x, y, z, a, e = pos
        x_adj, y_adj = self.calc_xy_adj(a)
        
        self.last_position[:] = [x + x_adj, y + y_adj, z, a, e]
            
        return list(self.last_position)
        
    def move(self, positions : list, last_position_): 
        last_position = last_position_
        transformed_positions = []
          
        for position in positions:
            self.splitter.build_move(last_position, position)
            while not self.splitter.traverse_complete:
                split_move = self.splitter.split()
                if split_move:
                    transformed_positions.append(split_move)
                else:
                    raise self.gcode.error(
                        "Concentricity Tolerance Compensation: Error splitting move ")
            last_position[:] = position
            

    def get_status(self, eventtime=None):
        pass
    
    # Calibration section
    cmd_CALIBRATE_DEFLECTION_ANGLE_help = "-TODO- Calibration Deflection Angle"
    def cmd_CALIBRATE_DEFLECTION_ANGLE(self, gcmd):
        pass
    
    cmd_CALIBRATE_DEFLECTION_RADIUS_help = "-TODO- Calibration Deflection Radius"
    def cmd_CALIBRATE_DEFLECTION_RADIUS(self, gcmd):
        pass
    
    
            
class MoveSplitter:
    def __init__(self, config, gcode, deflection_angle, deflection_radius):
        self.split_delta_xy = config.getfloat(
            'split_delta_xy', .025, minval=0.01)
        self.move_check_distance_a = config.getfloat(
            'move_check_distance_a', 5., minval=3.)
        self.gcode = gcode
        self.deflection_angle = deflection_angle
        self.deflection_radius = deflection_radius
        
        
    def build_move(self, prev_pos, next_pos):
        self.prev_pos = tuple(prev_pos)
        self.next_pos = tuple(next_pos)
        self.current_pos = list(prev_pos)
        
        self.x_offset, self.y_offset = self._calc_xy_offset(prev_pos)
        
        self.traverse_complete = False
        self.distance_checked = 0.
        axes_d = [self.next_pos[i] - self.prev_pos[i] for i in range(5)]
        self.total_move_length = math.sqrt(axes_d[3] * axes_d[3])
        self.axis_move = [not isclose(d, 0., abs_tol=1e-10) for d in axes_d]
      
    def calc_xy_adj(self, a_pos):
        calc_deflection_angle = math.radians(a_pos + self.deflection_angle)
        
        x_adj = self.deflection_radius * math.sin(calc_deflection_angle)
        y_adj = self.deflection_radius * math.cos(calc_deflection_angle)
        
        return x_adj, y_adj
      
    def _calc_xy_offset(self, pos):
        return self.calc_xy_adj(pos[3])
    
    def _set_next_move(self, distance_from_prev):
        t = distance_from_prev / self.total_move_length
        if t > 1. or t < 0.:
            raise self.gcode.error(
                "conecntricity_tolerance_compensation: Slice distance is negative "
                "or greater than entire move length")
        for i in range(5):
            if self.axis_move[i]:
                self.current_pos[i] = lerp(
                    t, self.prev_pos[i], self.next_pos[i])
                
    def split(self):
        if not self.traverse_complete:
            if self.axis_move[3]:
                # X and/or Y axis move, traverse if necessary
                while self.distance_checked + self.move_check_distance_a \
                        < self.total_move_length:
                    self.distance_checked += self.move_check_distance_a
                    self._set_next_move(self.distance_checked)
                    next_offset_x, next_offset_y = self._calc_xy_offset(self.current_pos)
                    if abs(next_offset_x -  self.x_offset) >= self.split_delta_xy or abs(next_offset_y - self.y_offset) >= self.split_delta_xy : 
                        self.x_offset = next_offset_x
                        self.y_offset = next_offset_y
                        
                        return self.current_pos[0] + self.x_offset, self.current_pos[1] + self.y_offset, \
                            self.current_pos[2], \
                            self.current_pos[3], self.current_pos[4]
            # end of move reached
            self.current_pos[:] = self.next_pos
            self.x_offset, self.y_offset = self._calc_xy_offset(self.current_pos)
            # Its okay to add Z-Offset to the final move, since it will not be
            # used again.
            self.current_pos[0] += self.x_offset
            self.current_pos[1] += self.y_offset 
            self.traverse_complete = True
            return self.current_pos
        else:
            # Traverse complete
            return None
        
def load_config(config):
    return ConcentricityToleranceCompansation(config)
