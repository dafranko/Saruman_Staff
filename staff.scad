$fn=64;
//scale ([0.5,0.5,0.5])
translate([0,0,250])
staff();



module staff(){
    base();
    wing_top();
    wing_bottom();
    wing_inner();
    
    //wing y-pos
    rotate([0,0,90])
    wing_top();
    rotate([0,0,90])
    wing_bottom();
    rotate([0,0,90])
    wing_inner();
    
    //wing neg x-pos
    rotate([0,0,180])
    wing_top();
    rotate([0,0,180])
    wing_bottom();
    rotate([0,0,180])
    wing_inner();
    
    //wing neg y-pos
    rotate([0,0,270])
    wing_top();
    rotate([0,0,270])
    wing_bottom();
    rotate([0,0,270])
    wing_inner();  
    
}
module base(){
cylinder(h=250, d=30);

translate([0,0,-300])
cylinder(h=300, d=30);

color([1,1,1])
translate([0,0,250])
sphere(d=80);   
}
module wing_top(){
    
    difference()
    {
       union(){
        //height of wing top 
        translate([0,-2.5,0])  
        cube([80,5,400]); 
       } 
       
       union(){
           
        translate([155,120,110])
        rotate([90,0,0])
        cylinder(h=250, d=240);
        
        //part of bottom wing  
        translate([195,120,-95])
        rotate([90,0,0])
        cylinder(h=250, d=325);
        
        translate([80,5,400])
        rotate([0,65,180])
        cube([140,15,350]);  
          
       }      
    }
}

module wing_bottom(){
    difference()
    {
      union(){
         translate([0,-2.5,-400])
         cube([80,5,400]); 
      }
   
      union(){
         translate([195,120,-95])
         rotate([90,0,0])
         cylinder(h=250, d=325);  
            
         translate([75,-5,-175])
         rotate([0,135,0])
         cube([225,15,350]);
       }
    }
    
    hull ()
    {
    translate([0,0,0])
    cylinder (h=1,d=30);
    
    translate([0,0,-150])
    cylinder (h=100,d=40);

    translate([0,0,-240])
    cylinder (h=1,d=30);
    }    
}
module wing_inner(){
difference(){
union(){
    difference(){

    translate([2.5,0,-300])
    rotate([0,0,45])
    cube([100,5,400]);
    
    translate([70,200,-95])
    rotate([90,0,45])
    cylinder(h=250, d=325);
       
    translate([5,195,180])
    rotate([90,0,45])
    cylinder(h=250, d=300);
    }
}//union
    translate([25,75,10])
    rotate([35,0,-45])
    cube([50,50,200]);

    translate([-75,20,-275])
    rotate([315,0,-45])
    cube([150,150,250]);
   
}//diff   
}