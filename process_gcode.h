//process_gcode.h

#ifndef Gcode_Included
#define Gcode_Included

#include <iostream>
#include <string>
#include <vector>
#include "point.h"

using std::vector;
using std::string;

typedef vector<Point> this_layer;
typedef vector<this_layer> all_layers; //vector<vector<Point>>
//typedef vector<vector<double>> pixel_layer;
//typedef vector<pixel_layer> model_pixels;


//refactor again? copied & pasted code for two functions...
//new function to convert string to double?
void get_perimeter_points(string line, this_layer &layer, int _layer_index);

void get_infill_points(string line, this_layer & layer, int _layer_index);

void make_new_layer(string line, all_layers & model_layers, int &layer_index);

//parse through gcode file & match regexes
all_layers match_regex(string gcodeFile, all_layers & model_layers, int &layer_index);


#endif //Gcode_Included