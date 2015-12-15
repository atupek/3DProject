//process bitmap
#ifndef Bitmap_Included
#define Bitmap_Included

#include "bitmap/bitmap_image.hpp"
#include "process_gcode.h"
#include "process_pixels.h"

void print_bitmap(pixel_layer &pix, int index, int num_pixel_rows, int num_pixel_columns);

void print_bitmap_from_pt_vector(this_layer &gcode_layer, int index, int num_pixel_rows, int num_pixel_columns);

void print_bitmap_lines_from_pt_vector(this_layer &gcode_layer, int index, int num_pixel_rows, int num_pixel_columns);

#endif //Bitmap_Included