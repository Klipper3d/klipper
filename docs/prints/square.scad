// Test square
//
// Generate STL using OpenSCAD:
//   openscad square.scad -o square.stl

square_width = 5;
square_size = 60;
square_height = 5;

difference() {
  cube([square_size, square_size, square_height]);
  translate([square_width, square_width, -1])
    cube([square_size-2*square_width, square_size-2*square_width, square_height+2]);
}
