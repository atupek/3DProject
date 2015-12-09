epsilon = 0.01;
layer_height = .2; //in mm
bridge_height = 2*layer_height;
extrusion_width = .5; //in mm
bridge_width = 2 * extrusion_width;
bridge_height_above_0 = 5.0;

x1 = 5.0;
y1 = 5.0;

x2 = 10.0;
y2 = 10.0;

pillar_x = 4.0;
pillar_y = 6.0;
pillar_z1 = 2.0;
pillar_z2 = 10.0;

pillar_height_above_0 = 10.0;
pillar_lw = 1.0;

pillar1_x = 10.0;
pillar1_y = 10.0;
pillar1_z1 = 0.0;
pillar1_z2 = 5.0;
pillar1_height = pillar1_z2 - pillar1_z1;
pillar1_height_above_0 = 0.0;

function distance(x1, y1, x2, y2) = sqrt(pow (x2 - x1, 2) + pow(y2 - y1, 2));

bridge_length = distance(x1, y1, x2, y2);
//echo (distance(x1, y1, x2, y2));

module bridge_platform(distance)
{
    translate([0, -bridge_width/2, -bridge_height])
    cube([bridge_length, bridge_width, bridge_height]);
}
//bridge_platform();

module bridge_pillar(pillar_height)
{
    cube([pillar_lw, pillar_lw, pillar_height], center = true);
}
//bridge_pillar();

module sample_bridge1()
{
    //translate so that endpt1 of bridge is at x1, y1
    //works because we don't use center = true on the bridge_platform module
    translate([x1, y1, 0])
    //translate so that bottom of bridge is at height of x1, y1 - bridge_height
    translate([0, 0, bridge_height_above_0])
    //rotate so that endpt2 of bridge is at x2, y2
    rotate([0, 0, atan2(y2, x2)])
    bridge_platform(bridge_length);
    //echo (atan2(y2, x2));
}
sample_bridge1();

module sample_pillar1()
{
    //translate so that the 'center' of the pillar is at x1, y1
    //and bottom of pillar is at pillar_height_above_0
    translate([pillar1_x, pillar1_y, 0])
    translate([0, 0, (pillar1_height)/2++pillar1_height_above_0-epsilon])
    bridge_pillar(pillar1_height);
}
sample_pillar1();

pillar2_x = 5.0;
pillar2_y = 5.0;
pillar2_z1 = 0.0;
pillar2_z2 = 5.0;
pillar2_height = pillar2_z2 - pillar2_z1;
pillar2_height_above_0 = 0.0;
module sample_pillar2()
{
    //translate so that the 'center' of the pillar is at x1, y1
    //and bottom of pillar is at pillar_height_above_0
    translate([pillar2_x, pillar2_y, 0])
    translate([0, 0, (pillar2_height)/2++pillar2_height_above_0-epsilon])
    bridge_pillar(pillar2_height);
}
sample_pillar2();

pillar3_x = 6.0;
pillar3_y = 6.0;
pillar3_z1 = 0.0;
pillar3_z2 = 5.0;
pillar3_height = pillar3_z2 - pillar3_z1;
pillar3_height_above_0 = 5.0;
module sample_pillar3()
{
    //translate so that the 'center' of the pillar is at x1, y1
    //and bottom of pillar is at pillar_height_above_0
    translate([pillar3_x, pillar3_y, 0])
    translate([0, 0, (pillar3_height)/2+pillar3_height_above_0-epsilon])
    bridge_pillar(pillar3_height);
}
sample_pillar3();

pillar4_x = 9.0;
pillar4_y = 9.0;
pillar4_z1 = 0.0;
pillar4_z2 = 5.0;
pillar4_height = pillar4_z2 - pillar4_z1;
pillar4_height_above_0 = 5.0;
module sample_pillar4()
{
    //translate so that the 'center' of the pillar is at x1, y1
    //and bottom of pillar is at pillar_height_above_0
    translate([pillar4_x, pillar4_y, 0])
    translate([0, 0, (pillar4_height)/2+pillar4_height_above_0-epsilon])
    bridge_pillar(pillar4_height);
}
sample_pillar4();

module test_cube()
{
    translate([5/2, 5/2, 5/2])
    cube(5, center = true);
}
//test_cube();