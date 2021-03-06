//slanted_pillar_test
//slanted pillar for thesis
//pillar from pt1 to pt2
//slanted_pillar(x1, y1, z1, x2, y2, z2)
//where z1 is above z2
$fa = 50;
$fn = 50;
radius = 1;

x_1 = 1;
y_1 = 2;
z_1 = 0;
x_2 = 4;
y_2 = 5;
z_2 = 3;

module circle1(x_coord, y_coord, z_coord)
{
    translate([x_coord, y_coord, z_coord])
    cylinder(.2, radius, radius);
}
//circle1(x_1, y_1, z_1);

module circle2(x_coord, y_coord, z_coord)
{
    translate([x_coord, y_coord, z_coord])
    cylinder(.2, radius, radius);
}
//circle2(x_2, y_2, z_2);

module slanted_pillar(x_coord1, y_coord1, z_coord1, x_coord2, y_coord2, z_coord2)
{
    hull()
    {
        circle1(x_coord1, y_coord1, z_coord1);
        circle2(x_coord2, y_coord2, z_coord2);
    }
}
slanted_pillar(x_1, y_1, z_1, x_2, y_2, z_2);