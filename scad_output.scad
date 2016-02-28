//scad pillars and bars generated by code

$fa = 50;
$fn = 50;
radius = 1;
delta = 2;
height = 0.4;

module circle1(x_coord, y_coord, z_coord)
{
	translate([x_coord, y_coord, z_coord])
	cylinder(.2, radius, radius);
}
module circle2(x_coord, y_coord, z_coord)
{
	translate([x_coord, y_coord, z_coord])
	cylinder(.2, radius, radius);
}
module slanted_pillar(x_coord1, y_coord1, z_coord1, x_coord2, y_coord2, z_coord2)
{
	hull()
	{
		circle1(x_coord1, y_coord1, z_coord1);
		circle2(x_coord2, y_coord2, z_coord2);
	}
}
module pillar(x_coord, y_coord, z_coord, z_height)
{
	translate([x_coord, y_coord, z_coord])
	linear_extrude(z_height)
	circle(radius);
}

//does not work for horizontal bridges, use bridge1
module bridge(x0, y0, x1, y1, z_height)
{
translate([0, 0, z_height])
linear_extrude(height)
polygon(points = [[x0+delta, y0], [x0-delta, y0], [x1-delta, y1], [x1+delta, y1]]);
}

//does not work for vertical bridges, use bridge
module bridge1(x0, y0, x1, y1, z_height)
{
translate([0, 0, z_height])
linear_extrude(height)
polygon(points = [[x0, y0+delta], [x0, y0-delta], [x1, y1-delta], [x1, y1+delta]]);
}

union()
{
//adding slanted pillar
	slanted_pillar(19, 13, 17, 19, 9, 15);
//adding slanted pillar
	slanted_pillar(13, 15, 22, 11, 15, 20);
//adding slanted pillar
	slanted_pillar(18, 20, 20, 18, 24, 17);
//adding point
	pillar(18, 24, 0, 17);
//adding point
	pillar(11, 15, 0, 20);
//adding point
	pillar(19, 9, 0, 15);
//adding point
	pillar(25, 20, 0, 18);
//adding bridge
	bridge(11, 15, 25, 20, 6);
	pillar(11, 15, 6, 14);
	pillar(25, 20, 6, 12);
//adding bridge
	bridge(19, 9, 11, 15, 5);
	pillar(19, 9, 5, 10);
	pillar(11, 15, 5, 15);
//adding bridge
	bridge(25, 20, 18, 24, 10);
	pillar(25, 20, 10, 8);
	pillar(18, 24, 10, 7);
}

