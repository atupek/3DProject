//generate_scaffolding.h
#ifndef Generate_Scaffolding_Included
#define Generate_Scaffolding_Included

#include "get_points/point.h"
#include "bridge.h"
#include "anchoring_segment.h"
//#include "pillar.h"
//#include "cube_primitive.h"

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
#include "event.h"


//void new_create_events(Point pt, double slope);
//void create_event_vector(Point pt, double slope);

//creates anchoring segment
void draw_line(Point pt, double slope);

void create_anchoring_segments(set<Point> &point_set, set<Bridge> &bridge_set, set<Anchoring_Segment> &anchor_segments,vector<double> &sweep_direction, int i);

void create_events(set<Anchoring_Segment> &_segments, set<Event> & events);

//void condense_events(set<Event> &events, set<Event> &condensed_events);

//sets of segements crossing sweep plane with anchoring segments(?)
void union_sets(set<Anchoring_Segment> & original_set, set<Anchoring_Segment> & new_set);

void difference_sets(set<Anchoring_Segment> & original_set, set<Anchoring_Segment> & remove_set);

//Pillar make_pillar(Point point1, double height);

//Cube_Primitive make_cube_primitive(Point point1, Point point2);

void snap(Bridge & best_bridge, set<Point> & points_supported_by_bridge);
//void snap(Bridge &best_bridge, set<Point> &points_supported_by_bridge, vector<Pillar> &pillars, vector<Cube_Primitive> &cubes, double dist_to_obj);

void generate_scaffolding(set<Point> points_needing_support);

//void generate_scaffolding(set<Point> pts_that_need_support);
//void generate_scaffolding_2(set<Point> pts_that_need_support);
//void generate_scaffolding1(vector<Point> pts_that_need_support);

#endif //Generate_Scaffolding_Included