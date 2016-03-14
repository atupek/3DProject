//process_pixels.h
#ifndef Pixels_Included
#define Pixels_Included

#include "process_gcode.h"
#include "process_layers.h"
#include <math.h>

//should probably only do this one layer at a time...
void initialize_pixel_vector(model_pixels & model, int num_rows, int num_columns);
void initialize_gridded_pixel_vector(model_pixels & model, int num_rows, int num_columns);

//this_layer is vector of Points
//pixel_layer is 2D vector of doubles
//only sending one layer at a time
void fill_pixel_vector(this_layer &gcode_layer, pixel_layer &pix);

void print_pixel_vector(pixel_layer & pix);

void compare_pixel_layers(pixel_layer &pix1, pixel_layer &pix2, pixel_layer & diff);

void fatten_lines(pixel_layer & pix1, pixel_layer & pix2, int num_rows, int num_columns);

void check_neighbors(pixel_layer & diff_pix, pixel_layer & comp_pix, pixel_layer & final_pix, int num_rows, int num_columns);

void list_points(pixel_layer & diff_pix, this_layer & pts_to_support, int num_rows, int num_columns, int z_level);

void drop_grid(pixel_layer & old_pts_to_support, pixel_layer & new_pts_to_support, int num_rows, int num_columns);
void list_gridded_points(pixel_layer & diff_pix, this_layer & pts_to_support, int num_rows, int num_columns, int z_level);
void fix_offset(all_layers & pts_to_support);

#endif //Pixels_Included