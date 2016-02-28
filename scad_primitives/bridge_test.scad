//bridge_primitive test
$fa = 50;
$fn = 50;

x2 = 0; //p1.x
y2 = 0; //p1.y
x3 = 10; //p2.x
y3 = 10; //p2.y
delta = 2;
x4 = 5;
y4 = 0;
bridge_height = 5; //bridge_height
bridge_height1 = 10;
height = 0.4;

//translate([0, 0, bridge_height])
//linear_extrude(.4)
//polygon(points = [[x0+delta, y0+delta], [x0-delta, y0-delta], [x1-delta, y1-delta], [x1+delta, y1+delta]]);
//polygon([[0,0],[100,0],[130,50],[30,50]]);
//polygon(points = [[3, 3], [-3, -3], [7, 7], [13, 13]]);
//polygon(points = [[x0+delta, y0], [x0-delta, y0], [x1-delta, y1], [x1+delta, y1]]);
/*
translate([0, 0, bridge_height1])
linear_extrude(.4)
polygon(points = [[x2+delta, y2], [x2-delta, y2], [x4-delta, y4], [x4+delta, y4]]);*/

module bridge(x0, y0, x1, y1, z_height)
{
    translate([0, 0, z_height])
    linear_extrude(height)
    polygon(points = [[x0+delta, y0], [x0-delta, y0], [x1-delta, y1], [x1+delta, y1]]);
}
%bridge(x2, y2, x3, y3, bridge_height);
%bridge(0, 0, 25, 20, 10);
%bridge(0, 0, 0, 10, 5);
//works unless bridge is horizontal...

//don't like this one...cut it
module bridge1(x0, y0, x1, y1, z_height)
{
    translate([0, 0, z_height])
    linear_extrude(height)
    polygon(points = [[x0+delta, y0+delta], [x0-delta, y0-delta], [x1-delta, y1-delta], [x1+delta, y1+delta]]);
}
bridge1(x2, y2, x3, y3, bridge_height);
bridge1(0, 0, 25, 20, 10);
bridge1(0, 0, 0, 10, 5);

//works unless bridge is vertical...
module bridge2(x0, y0, x1, y1, z_height)
{
    translate([0, 0, z_height])
    linear_extrude(height)
    polygon(points = [[x0, y0+delta], [x0, y0-delta], [x1, y1-delta], [x1, y1+delta]]);
}
bridge2(x2, y2, x3, y3, bridge_height);
bridge2(0, 0, 25, 20, 10);
bridge2(0, 0, 10, 0, 5);


