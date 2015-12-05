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
//using std::queue;
using std::priority_queue;
#include <functional> //for greater
#include <vector>
using std::vector; //vector of sweep directions
#include <algorithm> //for for_each

//creates anchoring segment
void draw_line(Point pt, double slope);

void create_anchoring_segments(set<Point> point_set, set<Bridge> bridge_set, vector<double> &sweep_direction, int i);

void create_events(Anchoring_Segment _segment);

//sets of segements crossing sweep plane with anchoring segments(?)
void union_sets(set<Anchoring_Segment> & original_set, set<Anchoring_Segment> & new_set);

void difference_sets(set<Anchoring_Segment> & original_set, set<Anchoring_Segment> & remove_set);

#endif //Generate_Scaffolding_Included