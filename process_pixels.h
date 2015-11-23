//process_pixels.h
#ifndef Pixels_Included
#define Pixels_Included

#include "process_gcode.h"
#include "process_layers.h"

//should probably only do this one layer at a time...
void initialize_pixel_vector(model_pixels & model, int num_rows, int num_columns);

//this_layer is vector of Points
//pixel_layer is 2D vector of doubles
//only sending one layer at a time
void fill_pixel_vector(this_layer &gcode_layer, pixel_layer &pix);

void print_pixel_vector(pixel_layer & pix);

void compare_pixel_layers(pixel_layer &pix1, pixel_layer &pix2, pixel_layer & diff);

#endif //Pixels_Included