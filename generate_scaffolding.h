//generate_scaffolding.h
#ifndef Generate_Scaffolding_Included
#define Generate_Scaffolding_Included

#include "point.h"
#include "bridge.h"

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

void draw_line(Point pt, double slope)
{
	//line should be centered at the point, and orthogonal to the slope, of a specific, given distance
	//should return a line segment, which is the anchoring segment
	//need anchoring segment class now that is a line segment with three points, a length, a height
	cout << "A LINE?" << endl;
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

//sets of segements crossing sweep plane with anchoring segments(?)
void union_sets()
{

}

#endif //Generate_Scaffolding_Included