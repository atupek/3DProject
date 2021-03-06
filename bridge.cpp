#include "bridge.h"
#include "get_points/process_layers.h"

Bridge::Bridge()
{
	Point this_point(0.0, 0.0, 0.0);
	p1 = this_point;
	p2 = this_point;
	height = 0;
	length = 0;
	pt1_open = true;
	pt2_open = true;
	score = -(std::numeric_limits<double>::infinity()); //not 0 since score can be negative!
	supported_points = {};
}

Bridge::Bridge(Point _p1, Point _p2, double _height)
{
	p1 = _p1;
	p2 = _p2;
	height = _height;
	//not sure if this function call will work...
	length = compute_distance(p1.x, p1.y, p2.x, p2.y);
	pt1_open = true;
	pt2_open = true;
	score = -(std::numeric_limits<double>::infinity()); //not 0 since score can be negative!
	supported_points = {}; 
}

Bridge::Bridge(Point _p1, Point _p2, double _length, double _height, bool _pt1_open, bool _pt2_open)
{
	p1 = _p1;
	p2 = _p2;
	length = _length;
	height = _height;
	pt1_open = _pt1_open;
	pt2_open = _pt2_open;
	score = -(std::numeric_limits<double>::infinity()); //not 0 since score can be negative!
	supported_points = {};
}

//overloaded << operator
ostream & operator << (ostream & os, const Bridge & obj)
{
    os << obj;
    return os;
}

bool operator < (const Bridge & lhs, const Bridge & rhs)
{
    return lhs.p1.x < rhs.p1.x;
}

bool operator > (const Bridge & lhs, const Bridge & rhs)
{
    return lhs.p1.x > rhs.p1.x;
}

bool operator == (const Bridge & lhs, const Bridge & rhs)
{
	if(lhs.p1 == rhs.p1 &&
	lhs.p2 == rhs.p2 &&
	lhs.length == rhs.length &&
	lhs.height == rhs.height &&
	lhs.pt1_open == rhs.pt1_open &&
	lhs.pt2_open == rhs.pt2_open &&
	lhs.score == rhs.score)
	{
		return true;
	}
	return false;
}

//print function to print point coordinates
void Bridge::print_bridge_members(ostream & os) const
{
	os << "Point1: ";
	p1.print_coords(os);
	os << "Point2: ";
	p2.print_coords(os);
	os << "Bridge length: " << length << endl;
	os << "Bridge height: " << height << endl;
	os << "P1 open: " << pt1_open << endl;
	os << "P2 open: " << pt2_open << endl;
}

void Bridge::print_bridge_pts_length(ostream & os) const
{
	os << "Point1: ";
	p1.print_coords(os);
	os << "Point2: ";
	p2.print_coords(os);
	os << "Bridge length: " << length << endl;
}

void Bridge::print_bridge_pts_height(ostream & os) const
{
	os << "Point1: ";
	p1.print_coords(os);
	os << "Point2: ";
	p2.print_coords(os);
	os << "Bridge height: " << height << endl;
}
