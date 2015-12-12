//anchoring_segment.h

#ifndef Anchoring_Segment_Included
#define Anchoring_Segment_Included

#include "point.h"
#include <vector>
using std::vector;
#include "bridge.h"

//anchoring_segments are composed of two endpoints, a slope, and any points that intersect it (a vector of points)
//and any bridges that intersect it (a vector of bridges)

//TODO: anchoring segment needs to change: --DONE
//one endpoint should be the event (midpt, which needs to be changed) --DONE
//so there should be two anchoring segments created from this, not one...fuck  --DONE

class Anchoring_Segment
{
public:
	Point endpt1;
	Point endpt2;
	double slope;
	vector<Point> intersected_points;
	vector<Bridge> intersected_bridges;
	double distance; //max length of bridge in mm
	double x1;
	double y1;

	double slope_squared;
	double delta_x;
	double delta_y;
	double new_x1;
	double new_y1;

	double height;

	Anchoring_Segment();
	//anchoring segment is created from a point and a slope
	//the point is either from the points that need support or the endpoints of the bridges that are already in place
	Anchoring_Segment(Point _eventpt, double _slope, bool negative);
	Anchoring_Segment(const Anchoring_Segment &anch);

	//function for sending point to ostream for printing
    void print_coords(ostream &)const;

	friend ostream & operator<<(ostream &, const Anchoring_Segment &);
    friend bool operator < (const Anchoring_Segment & lhs, const Anchoring_Segment & rhs);
    friend bool operator > (const Anchoring_Segment & lhs, const Anchoring_Segment & rhs);
};

#endif //Anchoring_Segment_Included