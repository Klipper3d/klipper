; Simple movement tests

; Start by homing the printer.
G28
G90
G1 F6000

; Z / X / Y moves
G1 Z1
G1 X1
G1 Y1

; Delayed moves
G1 Y2
G4 P100
G1 Y1.5
M400
G1 Y1

; diagonal moves
G1 X0 Y0
G1 X1 Z2
G1 X0 Y1 Z1

; extrude only moves
G1 E1
G1 E0

; Verify GET_POSITION works
GET_POSITION

; regular extrude move
G1 X0 Y0 E.01

; fan speeds
M106 S50
M106
M106 S90
M106 S0
M107

; coordinate manipulation
G92 Y-3
G1 Y-2
G91
G1 Y-1
