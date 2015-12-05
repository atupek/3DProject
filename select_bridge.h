//select_bridge.h
#ifndef Select_Bridge_Included
#define Select_Bridge_Included

#include "bridge.h"
#include "anchoring_segment.h"
#include "process_layers.h"
#include "point.h"
#include <set>
using std::set;

void select_bridge(set<Anchoring_Segment> segment);

double calculate_lmax(double height, double length);
double calculate_gain(double height, double length, int num_elements);
double calculate_gain_above_object(int num_elements, double height, double length, double h1, double h2);
double calculate_score(double gain, int num_elements, double lmax);
double calculate_distance(double x1, double x2, double y1, double y2);

#endif //Select_Bridge_Included