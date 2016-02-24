//select_bridge.h
#ifndef Select_Bridge_Included
#define Select_Bridge_Included

#include "bridge.h"
#include "anchoring_segment.h"
#include "get_points/process_layers.h"
#include "get_points/point.h"
#include <set>
using std::set;
#include <vector>
using std::vector;
#include "sweep_line.h"


double pt_line_dist(Point p1, Point p2, double x0, double y0);
double calc_z_diff(Point supported_pt, double temp_bridge_height);
double calc_horiz_dist(Point p1, Point p2, Point supported_pt);
double calculate_lmax(Point p1, Point p2, set<Point> supported_pts);
double calculate_gain(double height, double length, int num_elements);
double calculate_score(double gain, int num_elements, double lmax);

void check_collision();

void stable_merge_y(vector<point_seg_pair>::iterator first, vector<point_seg_pair>::iterator middle, vector<point_seg_pair>::iterator last);
void stable_merge_x(vector<point_seg_pair>::iterator first, vector<point_seg_pair>::iterator middle, vector<point_seg_pair>::iterator last);
void merge_sort_y(vector<point_seg_pair>::iterator first, vector<point_seg_pair>::iterator last);
void merge_sort_x(vector<point_seg_pair>::iterator first, vector<point_seg_pair>::iterator last);

double calc_dist(double x1, double y1, double x2, double y2);

void sorted_by_y(Sweep_line &line_to_sort);
void sorted_by_x(Sweep_line & line_to_sort);
void sort_sweep_lines(vector<Sweep_line> & sweep_lines, double sweep_slope);

Bridge select_bridge(vector<Sweep_line> & sweep_lines, double sweep_slope);



#endif //Select_Bridge_Included