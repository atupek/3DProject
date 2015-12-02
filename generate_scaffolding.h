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
	cout << "A LINE?" << endl;
}

void create_anchoring_segments(set<Point> point_set, vector<double> &sweep_direction, int i)
{
	double plane = sweep_direction[i];
	//for each Point in point_set, create anchor segment of length max_length orthoganl to sweep_direction, centered on Point
	//for_each(point_set.begin(), point_set.end(), draw_line(sweep_direction[i]));
	for(auto i = point_set.begin(); i != point_set.end(); i++)
	{
		draw_line(*i, plane);
	}
}

//sets of segements crossing sweep plane with anchoring segments(?)
void intersect_sets()
{

}

#endif //Generate_Scaffolding_Included