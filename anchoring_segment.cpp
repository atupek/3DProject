//anchoring_segment.cpp
#include "anchoring_segment.h"
#include "math.h" //for pow & sqrt

Anchoring_Segment::Anchoring_Segment(Point _midpt, double _slope)
{
	slope = -1/_slope;
	intersected_points = {_midpt}; //vector only contains _midpt right now.
	intersected_bridges = {}; //empty vector
	distance = 30; //max length of bridge in mm
	x1 = _midpt.x;
	y1 = _midpt.y;
	x2 = _midpt.y;
	y2 = _midpt.y;
	slope_squared = pow(slope, 2);
	delta_x = distance/sqrt(1+slope_squared);
	delta_y = (distance * slope)/sqrt(1+slope_squared);
	new_x1 = _midpt.x-delta_x;
	new_y1 = _midpt.y-delta_y;
	new_x2 = _midpt.x+delta_x;
	new_y2 = _midpt.y+delta_y;

	endpt1.x = new_x1;
	endpt1.y = new_y1;
	endpt1.extrude_amt = 0;
	endpt1.perimeter = true;

	endpt2.x = new_x2;
	endpt2.y = new_y2;
	endpt2.extrude_amt = 0;
	endpt2.perimeter = true;

}

//overloaded << operator
ostream & operator << (ostream & os, const Anchoring_Segment & obj)
{
    os << obj;
    return os;
}

bool operator < (const Anchoring_Segment & lhs, const Anchoring_Segment & rhs)
{
    return lhs.new_x1 < rhs.new_x1;
}

bool operator > (const Anchoring_Segment & lhs, const Anchoring_Segment & rhs)
{
    return lhs.new_x1 > rhs.new_x1;
}

//print function to print point coordinates
void Anchoring_Segment::print_coords(ostream & os) const
{
	os << endpt1.x << ", " << endpt1.y << " to " << endpt2.x << ", " << endpt2.y << endl;

}
