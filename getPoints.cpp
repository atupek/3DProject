//lines that we're looking for are in the form
//G1 Xnnnn Ynnnn Ennnn with options Fnnnn
//pull out X & Y coordinate as points
//pull out E as amount of filament to extrude
//(may need it later...)
//need to 'draw' line between points n, n-1
//at G1 Znnnn command, begin new layer

//while !eof
//get line
//match regex 1 (G1 Xnnnn Ynnnn Ennnn Fnnnn)
//if match, store x, y, e to file
//else continue
//match regex 2 (G1 Znnnnn)
//if match, start new layer
//else continue

//CURRENTLY WORKING ON:
//DEBUG: get the extrusion amount properly printing in the print_all function call in the print_num_pts_in_layer function --DONE!
//multiply x, y by ten & insert into 2D array 1 for filled w point, 0 for not filled  -- DONE
// draw line between points
//slope = m = (y2-y1)/(x2-x1)
//line (y-y1) = m * (x - x1)
//so need a function to 'draw' that line in the sparse array  --Using Bresenham Algorithm  --DONE
//TODO: Write Testing Data for 'printing out contents of pixel vector'  --DONE
//put pixel processing into own .cpp & .h --DONE
//put gcode processing into own .cpp & .h --DONE
//put layer processing into own .cpp & .h --DONE
//write code to find layer difference in points --DONE
//refactor some names to be more self-explanatory ie: this_layer is from gcode, make this obvious from name


#include "point.h"
#include "process_gcode.h"
#include "process_layers.h"
#include "process_pixels.h"
#include "bresenham.h"

//for printing bitmaps
#include "bitmap/bitmap_image.hpp"

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

char gcodeFile[256];
all_layers model_layers;

int layer_index = 0;

//2000 for 0.1mm resolution, use multiply_by_ten
//200 for 1.0mm resolution, don't use mulitply_by_ten
int num_pixel_rows = 200;
int num_pixel_columns = 200;

model_pixels model;

//gets the file name
void get_file_name()
{
	cout << "Enter the name of the gcode file: ";
	cin.getline(gcodeFile, 256);
}

void print_bitmap(pixel_layer &pix)
{
	bitmap_image image(num_pixel_rows, num_pixel_columns);

   // set background to white
   image.set_all_channels(255,255,255);

   image_drawer draw(image);

   //draw.pen_width(3);
   //draw.pen_color(255,0,0);
   //draw.circle(image.width() / 2, image.height() / 2,50);
   /*
   draw.pen_width(1);
   draw.pen_color(0,0,0);
   //draw.rectangle(50,50,150,150);
   draw.plot_pixel(50, 50);

   for(int i = 0; i < 100; i++)
   {
      draw.pen_width(1);
      draw.pen_color(0, 0, 0);
      draw.plot_pixel(i, i);
   }*/

   for(auto i = 0; i < pix.size(); i++)
	{
		draw.pen_width(1);
      	draw.pen_color(0, 0, 0);
		for(auto j = 0; j < pix[i].size(); j++)
		{
			if(pix[i][j] == 1.0)
			{
				//cout << "X"; //draw point if point is there
				//draw.pen_width(1);
      			//draw.pen_color(0, 0, 0);
				draw.plot_pixel(i, j);
			}
			else
			{
				//do nothing
				//cout << " "; //space if no point
			}
		}
	}


   image.save_image("output.bmp");
}

int main()
{

	get_file_name();
	all_layers this_model = match_regex(gcodeFile, model_layers, layer_index);

	all_layers converted_model = this_model;


	for(auto i = converted_model.begin(); i != converted_model.end(); i++)
	{
		multiply_by_ten(*i);
	}

	
	//create vector of pixel layers
	//takes a while for a 2000 x 2000 vector for 100 layers.  But, it works!
	cout << "Layer index in main: " << layer_index << endl;
	for(int i = 0; i<layer_index; i++)
	{
		initialize_pixel_vector(model, num_pixel_rows, num_pixel_columns);
	}

	//print_stuff(converted_model);
	//print_stuff(model_layers);
	//print_stuff(this_model);

	
	//fill_pixel_vector(converted_model[3], model[3]);
	fill_pixel_vector(model_layers[3], model[3]);
	cout << "Num layers in model: " << model.size() << endl;

	//print_pixel_vector(model[3]);

	//this_layer test_point_vec = this_model[3];

	//for(auto i = 0; i < test_point_vec.size()-1; i++)
	for(auto i = 0; i < this_model[3].size(); i++)
	{
		//bresenham(test_point_vec[i].x, test_point_vec[i+1].x, test_point_vec[i].y, test_point_vec[i+1].y, model[3]);
		bresenham(this_model[3][i].x, this_model[3][i+1].x, this_model[3][i].y, this_model[3][i+1].y, model[3]);
	}

	//cout << "***************************************************************************" << endl;
	//print_pixel_vector(model[3]);
	
	//compare_pixel_layers(model[3], model[3], model[3]);
	//print_pixel_vector(model[3]);
	print_bitmap(model[3]);

	return 0;
}