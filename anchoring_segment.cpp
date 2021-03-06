//anchoring_segment.cpp
#include "anchoring_segment.h"
#include "math.h" //for pow & sqrt
#include <limits> //for infinity

Anchoring_Segment::Anchoring_Segment()
{

}

Anchoring_Segment::Anchoring_Segment(Point _eventpt, double _slope, bool negative)
{
	distance = 30; //max length of bridge in mm
	//cout << "Point passed in: ";
	//_eventpt.print_coords(cout);
	if(_slope == 0)
	{
		slope = std::numeric_limits<double>::infinity();
		slope_squared = std::numeric_limits<double>::infinity();
		delta_x = 0;
		delta_y = distance;

		endpt1 = _eventpt;
		x1 = _eventpt.x;
		y1 = _eventpt.y;
		height = _eventpt.z;

		if(negative)
		{
			new_x1 = _eventpt.x-delta_x;
			//cout << "negative, new_x1: " << new_x1 << endl;
			new_y1 = _eventpt.y-delta_y;
			//cout << "negative, new_y1: " << new_y1 << endl;
		}
		else
		{
			new_x1 = _eventpt.x+delta_x;
			//cout << "positive, new_x1: " << new_x1 << endl;
			new_y1 = _eventpt.y+delta_y;
			//cout << "positive, new_y1: " << new_x1 << endl;
		}

		endpt2.x = new_x1;
		endpt2.y = new_y1;
		endpt2.z = _eventpt.z;
		endpt2.extrude_amt = 0;
		endpt2.perimeter = true;
	}
	else if(_slope == std::numeric_limits<double>::infinity())
	{
		slope = 0;
		slope_squared = 0;
		delta_x = distance;
		delta_y = 0;

		endpt1 = _eventpt;
		x1 = _eventpt.x;
		y1 = _eventpt.y;
		height = _eventpt.z;

		if(negative)
		{
			new_x1 = _eventpt.x-delta_x;
			//cout << "negative, new_x1: " << new_x1 << endl;
			new_y1 = _eventpt.y-delta_y;
			//cout << "negative, new_y1: " << new_y1 << endl;
		}
		else
		{
			new_x1 = _eventpt.x+delta_x;
			//cout << "positive, new_x1: " << new_x1 << endl;
			new_y1 = _eventpt.y+delta_y;
			//cout << "positive, new_y1: " << new_x1 << endl;
		}

		endpt2.x = new_x1;
		endpt2.y = new_y1;
		endpt2.z = _eventpt.z;
		endpt2.extrude_amt = 0;
		endpt2.perimeter = true;
	}
	else
	{
		slope = -1/_slope;
		slope_squared = pow(slope, 2);
		delta_x = distance/sqrt(1+slope_squared);
		delta_y = (distance * slope)/sqrt(1+slope_squared);

		endpt1 = _eventpt;
		x1 = _eventpt.x;
		y1 = _eventpt.y;
		height = _eventpt.z;

		if(negative)
		{
			new_x1 = _eventpt.x-delta_x;
			//cout << "negative, new_x1: " << new_x1 << endl;
			new_y1 = _eventpt.y-delta_y;
			//cout << "negative, new_y1: " << new_y1 << endl;
		}
		else
		{
			new_x1 = _eventpt.x+delta_x;
			//cout << "positive, new_x1: " << new_x1 << endl;
			new_y1 = _eventpt.y+delta_y;
			//cout << "positive, new_y1: " << new_x1 << endl;
		}

		endpt2.x = new_x1;
		endpt2.y = new_y1;
		endpt2.z = _eventpt.z;
		endpt2.extrude_amt = 0;
		endpt2.perimeter = true;
	}
	//slope = -1/_slope;
	intersected_points = {_eventpt}; //vector only contains _eventpt right now.
	intersected_bridges = {}; //empty vector for now
/*
	endpt1 = _eventpt;
	x1 = _eventpt.x;
	y1 = _eventpt.y;
	height = _eventpt.z;*/

	//slope_squared = pow(slope, 2);
	//delta_x = distance/sqrt(1+slope_squared);
	//delta_y = (distance * slope)/sqrt(1+slope_squared);

/*
	if(negative)
	{
		new_x1 = _eventpt.x-delta_x;
		//cout << "negative, new_x1: " << new_x1 << endl;
		new_y1 = _eventpt.y-delta_y;
		//cout << "negative, new_y1: " << new_y1 << endl;
	}
	else
	{
		new_x1 = _eventpt.x+delta_x;
		//cout << "positive, new_x1: " << new_x1 << endl;
		new_y1 = _eventpt.y+delta_y;
		//cout << "positive, new_y1: " << new_x1 << endl;
	}*/
/*
	endpt2.x = new_x1;
	endpt2.y = new_y1;
	endpt2.z = _eventpt.z;
	endpt2.extrude_amt = 0;
	endpt2.perimeter = true;*/

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
	intersected_points = anch.intersected_points;
}

//overloaded << operator
ostream & operator << (ostream & os, const Anchoring_Segment & obj)
{
    os << obj;
    return os;
}

bool operator < (const Anchoring_Segment & lhs, const Anchoring_Segment & rhs)
{
	//if((lhs.endpt1.x < rhs.endpt1.x) && (lhs.endpt1.y != rhs.endpt1.y) && (lhs.endpt2.x != rhs.endpt2.x) && (lhs.endpt2.y != rhs.endpt2.y))
	if(lhs.slope == std::numeric_limits<double>::infinity() && rhs.slope == std::numeric_limits<double>::infinity())
	{
		if(lhs.endpt2.y > rhs.endpt2.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(lhs.slope == 0 && rhs.slope == 0)
	{
		if(lhs.endpt2.x > rhs.endpt2.x)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(!(lhs.endpt1.x > rhs.endpt1.x) && (lhs.endpt2.x != rhs.endpt2.x) && (lhs.endpt2.y != rhs.endpt2.y))
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
	//if(endpt1 < endpt2)
	//{
		os << endpt1.x << ", " << endpt1.y << " to " << endpt2.x << ", " << endpt2.y << " at z = " << endpt1.z << endl;
	//}
	//else
	//{
	//	os << endpt2.x << ", " << endpt2.y << " to " << endpt1.x << ", " << endpt1.y << " at z = " << endpt1.z << endl;
	//}
	//os << "intersects sweep plane at point: " << intersect_pt.x << ", " << intersect_pt.y << endl;
}

//print intersecting points
void Anchoring_Segment::print_intersect_pts(ostream & os)const
{
	for(auto i = intersected_points.begin(); i != intersected_points.end(); i++)
	{
		os << i->x << ", " << i->y << ", at z= " << i->z << endl;
	}
}
