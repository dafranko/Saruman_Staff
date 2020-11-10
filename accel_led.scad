
$fn=64;

translate([0,0,0])
base_ring3();

//version 1
//updated the diameters for the ring
//new structure to hold accel

//version 2
//increase the outwalls and accel box

//version3
//outwalls were correct, accel was shrunk by 1mm

module base_accel3(){
    
    //creates the box for accel
    difference(){
        
        union(){
            difference(){
                translate([-11,-12.5,-0.5])
                cube([23,25,9]);
                translate([-10,-11.5,-5])
                cube([21,23,20]);
            } 
            translate([-11,-12.5,-0.5])
            cube([22,24,1]);
        }
        
        translate([-5,0,-5])
        cylinder(h=10,d=6);
    }
}



module base_ring3(){
    //to make the ring, make inner hollow
    translate([0,0,1])
    union(){
        difference(){
            translate([0,0,-1])
            cylinder(h=2,d=66.5);
            translate([0,0,-10])
            cylinder(h=20,d=51.2);
            
            //drill holes on the ring
            translate([30.5,0,-5])
            cylinder(h=10,d=4);
            translate([-30.5,0,-5])
            cylinder(h=10,d=4);   
            translate([-22.5,-20,-5])
            cylinder(h=10,d=4);
        }
    }
    
    //holds outer and inner rings walls
    difference(){
        union(){
            difference(){
                cylinder(h=7,d=70);
                translate([0,0,-25])
                cylinder(h=50,d=48);
            }//diff
        }//union
        
        union(){
            difference(){
                cylinder(h=75,d=66.5);
                       
                translate([0,0,0])
                cylinder(h=75,d=51.2);
            }//diff
        }//union
    }//diff
    
    //structure support for accel
    
    translate([-3.5,12.5,3])
    cube([7,12,2]);
    translate([-3.5,-25,1])
    cube([7,13,2]);
    translate([24,-3.5,1])
    rotate([0,0,90])
    cube([7,13,2]);
    translate([-24,3.5,1])
    rotate([0,0,270])
    cube([7,13,2]);
    
    base_accel3();
}