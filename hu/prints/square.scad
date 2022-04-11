// Test square
//
// Generate STL using OpenSCAD:
//   openscad square.scad -o square.stl

square_width = 5;
square_size = 60;
square_height = 5;

module hollow_square() {
    difference() {
        cube([square_size, square_size, square_height]);
        translate([square_width, square_width, -1])
            cube([square_size-2*square_width, square_size-2*square_width,
                  square_height+2]);
    }
}

module notch() {
    CUT = 0.01;
    depth = .5;
    width = 1;
    translate([-depth, -width, -CUT])
        cube([2*depth, 2*width, square_height + 2*CUT]);
}

module square_with_notches() {
    difference() {
        // Start with initial square
        hollow_square();
        // Remove four notches on inside perimeter
        translate([square_width, square_size/2 - 4, 0])
            notch();
        translate([square_size/2, square_size - square_width, 0])
            rotate([0, 0, 90])
                notch();
        translate([square_size - square_width, square_size/2, 0])
            notch();
        translate([square_size/2, square_width, 0])
            rotate([0, 0, 90])
                notch();
    }
}

square_with_notches();
