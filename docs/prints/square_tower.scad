// Calibration test tower
//
// Generate STL using OpenSCAD:
//   openscad square_tower.scad -o square_tower.stl

square_width = 5;
square_size = 60;
square_height = 50;
antiwarp_height = .8;
antiwarp_radius = 5;
CUT = 0.01;

module notch(x, y, rot) {
    depth = .5;
    width = 1;
    translate([x, y, 0])
        rotate([0, 0, rot])
            translate([-depth, -width, -CUT])
                cube([2*depth, 2*width, square_height + 2*CUT]);
}

module square_with_notches() {
    difference() {
        // Start with initial square
        cube([square_size, square_size, square_height]);
        // Remove four notches on inside perimeter
        notch(square_width, square_size/2 - 4, 0);
        notch(square_size - square_width, square_size/2, 0);
        notch(square_size/2, square_width, 90);
        notch(square_size/2, square_size - square_width, 90);
    }
}

module anti_warp_feet() {
    module anti_warp_cylinder() {
        cylinder(r=antiwarp_radius, h=antiwarp_height, $fs=.5);
    }
    dist = antiwarp_radius / 2.5;
    translate([dist, dist, 0])
        anti_warp_cylinder();
    translate([square_size-dist, dist, 0])
        anti_warp_cylinder();
    translate([dist, square_size-dist, 0])
        anti_warp_cylinder();
    translate([square_size-dist, square_size-dist, 0])
        anti_warp_cylinder();
}

module hollow_square() {
    difference() {
        union() {
            square_with_notches();
            anti_warp_feet();
        }
        translate([square_width, square_width, -CUT])
            cube([square_size-2*square_width, square_size-2*square_width,
                  square_height + 2*CUT]);
    }
}

hollow_square();
