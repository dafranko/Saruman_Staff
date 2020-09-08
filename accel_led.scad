$fn=64;

translate([0,0,0])
base_ring();


module base_accel(){
    
    //creates the box for accel
    difference(){
        
        union(){
            difference(){
                translate([-11,-12,-0.5])
                cube([22,24,9]);
                translate([-10.5,-11.5,-5])
                cube([21,23,20]);
            } 
            translate([-11,-12,-0.5])
            cube([22,24,1]);
        }
        
        translate([-7,0,-5])
        cylinder(h=10,d=6);
    }
}

module base_ring(){
    
    //to make the ring, make inner hollow
    difference(){
        translate([0,0,-1])
        cylinder(h=2,d=74);
        translate([0,0,-10])
        cylinder(h=20,d=52);
        
        translate([30.5,0,-5])
        cylinder(h=10,d=6);
        translate([-30.5,0,-5])
        cylinder(h=10,d=6);   
        translate([-22.5,-21,-5])
        cylinder(h=10,d=6);
    }
    
    //holds outer and inner rings
    difference(){
        union(){
            difference(){
                cylinder(h=7,d=74);
                
                translate([0,0,-25])
                cylinder(h=50,d=52);
            }//diff
        }//union
        
        union(){
            difference(){
                cylinder(h=75,d=69);
                       
                translate([0,0,0])
                cylinder(h=75,d=54);
            }//diff
        }//union
    }//diff
    
    //creates the structure for the accel
    difference(){
        union(){
            rotate([0,90,0])
            translate([-3,0,-26])
            cylinder(h=52,d=7);

            rotate([0,90,90])
            translate([-3,0,-26])
            cylinder(h=52,d=7);
        }
    
        translate([-11,-12,-0.5])
        cube([22,24,10]);
    
    }
    
    base_accel();  
}