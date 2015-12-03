//anchoring_segment.h

#ifndef Anchoring_Segment_Included
#define Anchoring_Segment_Included
#include "point.h"
#include <vector>
using std::vector;
#include "bridge.h"

//anchoring_segments are composed of two endpoints, a slope, and any points that intersect it (a vector of points)

class Anchoring_Segment
{
public:
	Point endpt1;
	Point endpt2;
	double slope;
	vector<Point> intersected_points;
	vector<Bridge> intersected_bridges;

	Anchoring_Segment(vector<Point> _intersection_points, vector<Bridge> _intersection_bridges, double slope);
};

#endif //Anchoring_Segment_Included