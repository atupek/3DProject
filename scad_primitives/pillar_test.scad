//pillar primitive test
$fa=50;
$fn=50;

radius = 1;
x=5;
y=5;
z_height = 5;
bridge_height = 2;

x1=7;
y1=7;
z_height1 = 5;
bridge_height1 = 4;

module pillar(x_coord, y_coord, z_coord, z_height)
{
    translate([x_coord, y_coord, z_coord])
    linear_extrude(z_height)
    circle(radius);
}
pillar(x, y, z_height, bridge_height);
pillar(x1, y1, z_height1, bridge_height1);

/*
module pillar1()
{
    translate([x1, y1, z_height1])
    linear_extrude(bridge_height1)
    circle(radius);
}
pillar1();*/

union()
{
    pillar(x, y, z_height, bridge_height);
    pillar(x1, y1, z_height1, bridge_height1);
    pillar(10, 10, 10, 10);
}
