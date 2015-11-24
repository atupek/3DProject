//process_bitmap.cpp
#include "bitmap/bitmap_image.hpp"
#include "process_bitmap.h"
#include <string>
using std::string;
using std::to_string;

void print_bitmap(pixel_layer &pix, int index, int num_pixel_rows, int num_pixel_columns)
{
	string bitmap_name = "output" + to_string(index) + ".bmp";
	bitmap_image image(num_pixel_rows, num_pixel_columns);

   // set background to white
   image.set_all_channels(255,255,255);

   image_drawer draw(image);

   for(auto i = 0; i < pix.size(); i++)
	{
		draw.pen_width(1);
      	//draw.pen_color(0, 0, 0);
		for(auto j = 0; j < pix[i].size(); j++)
		{
			if(pix[i][j] == 1.0) //not for printing differences
			{
				draw.pen_color(0, 0, 0);
				draw.plot_pixel(i, j);
			}
			if(pix[i][j] == 3.0) // n
			{
				draw.pen_color(255, 0, 0); //nothing beneath, not okay, red
				draw.plot_pixel(i, j);
			}
			if(pix[i][j] == 2.0)
			{
				draw.pen_color(0, 255, 0); //nothing above, is okay, green
				draw.plot_pixel(i, j);
			}
		}
	}


   image.save_image(bitmap_name);
}

void print_bitmap_from_pt_vector(this_layer &gcode_layer, int index, int num_pixel_rows, int num_pixel_columns)
{
	string bitmap_name = "output" + to_string(index) + ".bmp";
	bitmap_image image(num_pixel_rows, num_pixel_columns);

   // set background to white
   image.set_all_channels(255,255,255);

   image_drawer draw(image);

   for(auto i = gcode_layer.begin(); i != gcode_layer.end()-1; i++)
	{
		draw.pen_width(1);
      	draw.pen_color(0, 0, 0);
		draw.plot_pixel(i->x, i->y);
	}


   image.save_image(bitmap_name);

}

void print_bitmap_lines_from_pt_vector(this_layer &gcode_layer, int index, int num_pixel_rows, int num_pixel_columns)
{
	string bitmap_name = "output" + to_string(index) + ".bmp";
	bitmap_image image(num_pixel_rows, num_pixel_columns);

   // set background to white
   image.set_all_channels(255,255,255);

   image_drawer draw(image);

   for (auto i = 0; i < gcode_layer.size()-1; i++)
   //for(auto i = gcode_layer.begin(); i != gcode_layer.end()-1; i++)
	{
		draw.pen_width(1);
      	draw.pen_color(0, 0, 0);
		//draw.line_segment(i->x, i->y, (i++)->x, (i++)->y);
		draw.line_segment(gcode_layer[i].x, gcode_layer[i].y, gcode_layer[(i+1)].x, gcode_layer[(i+1)].y);
	}


   image.save_image(bitmap_name);

}
