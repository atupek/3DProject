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

Bridge new_select_bridge(vector<Sweep_line> & sweep_lines, double sweep_slope);
void sort_sweep_lines(vector<Sweep_line> & sweep_lines, double sweep_slope);
double new_calc_lmax(Point p1, Point p2, set<Point> supported_pts);

double calculate_gain(double height, double length, int num_elements);
double calculate_score(double gain, int num_elements, double lmax);
double calc_dist(double x1, double y1, double x2, double y2);

bool on_same_sweep_line(Point p1, Point p2, double sweep_slope);

vector<Anchoring_Segment> set_up_sort_segments_by_z(set<Anchoring_Segment> &segments);
void sort_segments_by_z(vector<Anchoring_Segment> & segment);
void stable_merge_z(vector<Anchoring_Segment>::iterator first, vector<Anchoring_Segment>::iterator middle, vector<Anchoring_Segment>::iterator last);
void merge_sort_z(vector<Anchoring_Segment>::iterator first, vector<Anchoring_Segment>::iterator last);

vector<Anchoring_Segment> set_up_sort_segments_by_y(set<Anchoring_Segment> &segments);
void sort_segments_by_y(vector<Anchoring_Segment> & segment);
//void stable_merge_y(vector<Anchoring_Segment>::iterator first, vector<Anchoring_Segment>::iterator middle, vector<Anchoring_Segment>::iterator last);
//void merge_sort_y(vector<Anchoring_Segment>::iterator first, vector<Anchoring_Segment>::iterator last);
void stable_merge_y(vector<point_seg_pair>::iterator first, vector<point_seg_pair>::iterator middle, vector<point_seg_pair>::iterator last);
void merge_sort_y(vector<point_seg_pair>::iterator first, vector<point_seg_pair>::iterator last);
void sorted_by_y(Sweep_line &line_to_sort);

#endif //Select_Bridge_Included