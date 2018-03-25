$fn=60;

blind_gear_28();


module blind_gear_28(){

    translate([0,5.5,0]){
    linear_extrude(3){
    text("waveb", valign="center", halign="center", size=5);};};

    translate([0,-5,0]){
    linear_extrude(3){
    text("30mm", valign="center", halign="center", size=5);};};


    difference(){
     // Beads
        
    size = 15;
    cylinder(r=size, h=5, center=true);
    size = 15;
    bead = 3;
    cord_beads(bead,size);
    rotate(45, [0,0,1] ){cord_beads(bead,size);};
    rotate(22.5, [0,0,1] ){cord_beads(bead,size);};
    rotate(45+22.5, [0,0,1] ){cord_beads(bead,size);};


    // donut
    rotate_extrude()
    translate([size, 0, 0])
    circle(r = 1);

    // center hole
    difference(){
        cylinder(r=2.6, h=10, center=true);
       translate([0,-4.55,0]){cube([10,5,12], center=true);};
    }
    };

    module cord_beads(r, d){
    translate([d,0,0]){
        sphere(r);
    };

    translate([-d,0,0]){
        sphere(r);
    };

    translate([0,d,0]){
        sphere(r);
    };

    translate([0,-d,0]){
        sphere(r);
    };

    }
}