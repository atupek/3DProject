//anchoring_segment.cpp
#include "anchoring_segment.h"
#include "math.h" //for pow & sqrt

Anchoring_Segment::Anchoring_Segment(Point _midpt, double _slope)
{
	slope = -1/_slope;
	intersected_points = {}; //empty vector
	intersected_bridges = {}; //empty vector
	distance = 30; //max length of bridge in mm
	x1 = _midpt.x;
	y1 = _midpt.y;
	x2 = _midpt.y;
	y2 = _midpt.y;
	slope_squared = pow(slope, 2);
	delta_x = distance/sqrt(1+slope_squared);
	delta_y = (distance * slope)/sqrt(1+slope_squared);
	new_x1 = _midpt.x+delta_x;
	new_y1 = _midpt.y+delta_y;
	new_x2 = _midpt.x-delta_x;
	new_y2 = _midpt.y-delta_y;

	endpt1.x = new_x1;
	endpt1.y = new_y1;
	endpt1.extrude_amt = 0;
	endpt1.perimeter = true;

	endpt2.x = new_x2;
	endpt2.y = new_y2;
	endpt2.extrude_amt = 0;
	endpt2.perimeter = true;

}
