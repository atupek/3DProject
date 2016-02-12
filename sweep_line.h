//sweep_plane class header
#ifndef Sweep_Line_Included
#define Sweep_Line_Included

#include "get_points/point.h"
#include "anchoring_segment.h"
#include <utility>
using std::pair;
using std::get;
#include <vector>
using std::vector;

typedef pair<Point, Anchoring_Segment> point_seg_pair;

//sweep plane consists of a set of intersection points
//and associated anchoring segments

class Sweep_line
{
public:

	//set<Point> intersected_points;
	//set<Anchoring_Segment> associated_segments;

	vector<point_seg_pair> intersected_points;

	Sweep_line();

    //function for sending sweep_line to ostream for printing
    void print_sweep_line_members(ostream &)const;
/*
    friend ostream & operator<<(ostream &, const Sweep_line &);
    friend bool operator < (const Sweep_line & lhs, const Sweep_line & rhs);
    friend bool operator > (const Sweep_line & lhs, const Sweep_line & rhs);
    friend bool operator == (const Sweep_line & lhs, const Sweep_line & rhs);*/
};

#endif //Sweep_Line_Included