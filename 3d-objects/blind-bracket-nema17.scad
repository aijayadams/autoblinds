$fn=60;

use <blind-gear.scad>



difference(){
    
    minkowski(){
        union(){
            cube([44,44,44], center=true);
            translate([17,0,-26]){
                cube([10,10,10], center=true);
            };
            translate([17,0,26]){
                cube([10,10,10], center=true);
            };

        };
        sphere(r=1);
    };
    translate([-30,0,0]){gearbox_cutter();};
    translate([-32,0,0]){rotate(90,[0,1,0]){Nema17(30);};};
    translate([0,0,28]){rotate(90,[0,1,0]){cylinder(r=1.5, h=39);};};
    translate([0,0,-28]){rotate(90,[0,1,0]){cylinder(r=1.5, h=39);};};

    translate([0,0,0]){rotate(90,[0,1,0]){cylinder(r=2.8, h=60);};};



};






module gearbox_cutter(){
  rotate(90,[0,1,0]){
        translate([-7,0,29]){
            //cylinder(r=12/2, h=25);
            translate([0,0,5]){
                //cylinder(r=34/2, h=5);
                 translate([-50,-34/2,0]){
                     cube([100,34,7]);
                 };
            };
        };
        //cylinder(r=38/2, h=30);
    };

};



module Nema17(motor_height){
	difference(){
//motor
		union(){
			translate([0,0,motor_height/2]){
				intersection(){
					cube([42.3,42.3,motor_height], center = true);
					rotate([0,0,45]) translate([0,0,-1]) cube([54, 54 ,motor_height+2], center = true);
				}
			}
			translate([0, 0, motor_height-1]) cylinder(h=3, r=11, $fn=24);
			translate([0, 0, motor_height]) cylinder(h=20, r=2.5, $fn=24);
		}
//screw holes
	}
}
