//generate_scaffolding.h
#ifndef Generate_Scaffolding_Included
#define Generate_Scaffolding_Included

#include "point.h"
#include "bridge.h"
#include "anchoring_segment.h"

#include <iostream>
using std::cout;
using std::endl;

#include <set>
#include <queue>
using std::set;
using std::queue;
#include <vector>
using std::vector; //vector of sweep directions
#include <algorithm> //for for_each

set<Anchoring_Segment> segments;
queue<Point> events;

void draw_line(Point pt, double slope)
{
	//line should be centered at the point, and orthogonal to the slope, of a specific, given distance
	//should return a line segment, which is the anchoring segment
	cout << "A LINE:" << endl;
	Anchoring_Segment new_segment(pt, slope);
	new_segment.print_coords(cout);
	segments.insert(new_segment);
}

void create_anchoring_segments(set<Point> point_set, set<Bridge> bridge_set, vector<double> &sweep_direction, int i)
{
	double plane = sweep_direction[i];
	//for each Point in point_set, create anchor segment of length max_length orthoganl to sweep_direction, centered on Point
	//for_each(point_set.begin(), point_set.end(), draw_line(sweep_direction[i]));
	for(auto i = point_set.begin(); i != point_set.end(); i++)
	{
		draw_line(*i, plane);
	}
	for(auto i = bridge_set.begin(); i != bridge_set.end(); i++)
	{
		draw_line(i->p1, plane);
		draw_line(i->p2, plane);
	}
}

void create_events(Anchoring_Segment _segment)
{
	//get intersecting points out of the anchoring segment
	for(auto i = _segment.intersected_points.begin(); i != _segment.intersected_points.end(); i++)
	{
		events.push(*i);
	}
	//get intersecting bridge points out of the anchoring segment
	for(auto i = _segment.intersected_bridges.begin(); i != _segment.intersected_bridges.end(); i++)
	{
		events.push(i->p1);
		events.push(i->p2);
	}
}

//sets of segements crossing sweep plane with anchoring segments(?)
void union_sets()
{

}

#endif //Generate_Scaffolding_Included