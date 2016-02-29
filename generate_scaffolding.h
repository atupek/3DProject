//generate_scaffolding.h
#ifndef Generate_Scaffolding_Included
#define Generate_Scaffolding_Included

#include "get_points/point.h"
#include "bridge.h"
#include "anchoring_segment.h"
//#include "pillar.h"
//#include "cube_primitive.h"
#include "sweep_line.h"

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
#include <string>
using std::string;

//creates anchoring segment
void draw_line(Point pt, double slope, set<Anchoring_Segment> &anchor_segments);

void create_anchoring_segments(set<Point> &point_set, set<Bridge> &bridge_set, set<Anchoring_Segment> &anchor_segments,vector<double> &sweep_direction, int i);

void create_events(set<Anchoring_Segment> &_segments, set<Event> & events);

double calculate_x_intersection(double y_intercept1, double y_intercept2, double slope1, double slope2);
double calculate_y_intersection(double y_intercept1, double y_intercept2, double slope1, double slope2);

void find_intersections(set<Event> & events, vector<double> sweep_directions, int sweep_index, vector<Sweep_line> &sweep_line_vec);

//sets of segements crossing sweep plane with anchoring segments
void union_sets(set<Anchoring_Segment> & original_set, set<Anchoring_Segment> & new_set);

void difference_sets(set<Anchoring_Segment> & original_set, set<Anchoring_Segment> & remove_set);

void difference_pt_sets(set<Point> & original_set, set<Point> & remove_set);
Point find_closest(Point bridge_p1, Point bridge_p2, Point p3);

//Pillar make_pillar(Point point1, double height);

//Cube_Primitive make_cube_primitive(Point point1, Point point2);

void remove_supported_pt_from_active_set(Point supported_pt, set<Point> & active_pts);
void snap(Bridge & best_bridge, set<Point> & active_pts);

void send_remaining_points_to_scad(set<Point> &active_pts);
void setup_scad();
void output_pts_to_support_to_scad(set<Point> &active_pts, string color);

void generate_scaffolding(set<Point> points_needing_support);


#endif //Generate_Scaffolding_Included