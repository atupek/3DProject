//anchoring_segment.cpp
#include "anchoring_segment.h"
#include "math.h" //for pow & sqrt

Anchoring_Segment::Anchoring_Segment()
{

}

Anchoring_Segment::Anchoring_Segment(Point _eventpt, double _slope, bool negative)
{
	slope = -1/_slope;
	intersected_points = {_eventpt}; //vector only contains _eventpt right now.
	intersected_bridges = {}; //empty vector for now
	distance = 30; //max length of bridge in mm
	endpt2 = _eventpt;
	x1 = _eventpt.x;
	y1 = _eventpt.y;
	height = _eventpt.z;

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
	endpt1.z = _eventpt.z;
	endpt1.extrude_amt = 0;
	endpt1.perimeter = true;

	intersect_pt.x = 0.0;
	intersect_pt.y = 0.0;
	intersect_pt.z = 0.0;
}

//copy constructor
Anchoring_Segment::Anchoring_Segment(const Anchoring_Segment &anch)
{
	endpt1 = anch.endpt1;
	endpt2 = anch.endpt2;
	intersect_pt = anch.intersect_pt;
	slope = anch.slope;
	height = anch.height;
}

//overloaded << operator
ostream & operator << (ostream & os, const Anchoring_Segment & obj)
{
    os << obj;
    return os;
}

bool operator < (const Anchoring_Segment & lhs, const Anchoring_Segment & rhs)
{
	if(lhs.endpt1.x < rhs.endpt1.x && lhs.endpt1.y != rhs.endpt1.y)
	{
		return true;
	}
    return false;
}

bool operator > (const Anchoring_Segment & lhs, const Anchoring_Segment & rhs)
{
    if(lhs.endpt1.x > rhs.endpt1.x && lhs.endpt1.y != rhs.endpt1.y)
	{
		return true;
	}
    return false;
}

//print function to print point coordinates
void Anchoring_Segment::print_coords(ostream & os) const
{
	if(endpt1 < endpt2)
	{
		os << endpt1.x << ", " << endpt1.y << " to " << endpt2.x << ", " << endpt2.y << " at z = " << endpt1.z << endl;
	}
	else
	{
		os << endpt2.x << ", " << endpt2.y << " to " << endpt1.x << ", " << endpt1.y << " at z = " << endpt1.z << endl;
	}
	os << "intersects sweep plane at point: " << intersect_pt.x << ", " << intersect_pt.y << endl;
}
