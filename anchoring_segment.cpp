//anchoring_segment.cpp
#include "anchoring_segment.h"
#include "math.h" //for pow & sqrt

Anchoring_Segment::Anchoring_Segment(Point _eventpt, double _slope, bool negative)
{
	slope = -1/_slope;
	intersected_points = {_eventpt}; //vector only contains _eventpt right now.
	intersected_bridges = {}; //empty vector
	distance = 30; //max length of bridge in mm
	endpt2 = _eventpt;
	x1 = _eventpt.x;
	y1 = _eventpt.y;

	slope_squared = pow(slope, 2);
	delta_x = distance/sqrt(1+slope_squared);
	delta_y = (distance * slope)/sqrt(1+slope_squared);

	if(negative)
	{
		new_x1 = _eventpt.x-delta_x;
		new_y1 = _eventpt.y-delta_y;
	}
	else
	{
		new_x1 = _eventpt.x+delta_x;
		new_y1 = _eventpt.y+delta_y;
	}

	endpt1.x = new_x1;
	endpt1.y = new_y1;
	endpt1.extrude_amt = 0;
	endpt1.perimeter = true;

	height = 0.0; //TODO: GET THIS FIXED/FIGURED OUT

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
	if(endpt1 < endpt2)
	{
		os << endpt1.x << ", " << endpt1.y << " to " << endpt2.x << ", " << endpt2.y << endl;
	}
	else
	{
		os << endpt2.x << ", " << endpt2.y << " to " << endpt1.x << ", " << endpt1.y << endl;
	}
}
