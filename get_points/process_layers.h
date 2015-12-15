//process_layers.h
#ifndef Layers_Included
#define Layers_Included

#include "process_gcode.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
#include <cstdlib>
#include <string>
using std::string;
using std::stod;
#include <regex>
using std::regex;
using std::smatch;
using std::regex_search;
#include <vector>
using std::vector;
#include <sstream>
using std::istringstream;
using std::istream_iterator;
#include <math.h> //for sqrt & lrint (round & cast to long int)


typedef vector<vector<double>> pixel_layer;
typedef vector<pixel_layer> model_pixels;


double compute_distance(double x1, double y1, double x2, double y2);
void print_distance(this_layer layer);
void multiply_by_ten(this_layer &layer);
void print_stuff(all_layers &model);

void multiply_by_two(this_layer & layer);

#endif //Layers_Included