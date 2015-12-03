//anchoring_segment.cpp
#include "anchoring_segment.h"

Anchoring_Segment::Anchoring_Segment(vector<Point> _intersection_points, vector<Bridge> _intersection_bridges, double _slope)
{
	slope = -1/slope;
	intersected_points = _intersection_points;
	intersected_bridges = _intersection_bridges;
	endpt1;
	endpt2;
}