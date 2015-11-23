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
//refactor print_bitmap into own .h & .cpp
//write code to show layer difference in points for test0.gcode layers

//Demo code for Monday:
// fill 2000 x 2000 vector with points from two subsequent layers
// print bitmap for layer1 & layer 2  --DONE
// find binary difference between layers --DONE
// print bitmap of difference --DONE


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
using std::to_string;
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
int num_pixel_rows = 2000;
int num_pixel_columns = 2000;

model_pixels model;

//gets the file name
void get_file_name()
{
	cout << "Enter the name of the gcode file: ";
	cin.getline(gcodeFile, 256);
}

void print_bitmap(pixel_layer &pix, int index)
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

int main()
{

	get_file_name();

	//match regex & create points, load them into layers, specified by the layer index
	all_layers this_model = match_regex(gcodeFile, model_layers, layer_index);

	all_layers converted_model = this_model;

	//multiply x, y, & e by 10 for the higher resolution
	for(auto i = converted_model.begin(); i != converted_model.end(); i++)
	{
		multiply_by_ten(*i);
	}

	
	//create vector of pixel layers, these are empty to begin with
	//and NEED TO BE POPULATED
	//takes a while for a 2000 x 2000 vector for 100 layers.  But, it works!
	for(int i = 0; i<layer_index; i++)
	{
		initialize_pixel_vector(model, num_pixel_rows, num_pixel_columns);
	}

	//print x, y coordinates of Point vector
	//set to either just print the coordinates,
	//or to print all of the Point's data members
	//print_stuff(converted_model);
	
	//not sure why this isn't working...
	fill_pixel_vector(converted_model[3], model[3]);

/*
	fill_pixel_vector(model_layers[3], model[3]);
	fill_pixel_vector(model_layers[4], model[4]);*/
	cout << "Num layers in model: " << model.size() << endl;
/*
	//draw lines between the points using bresenham algorithm
	for(auto i = 0; i < this_model[3].size(); i++)
	{
		bresenham(this_model[3][i].x, this_model[3][i+1].x, this_model[3][i].y, this_model[3][i+1].y, model[3]);
	}
	
	for(auto i = 0; i < this_model[4].size(); i++)
	{
		bresenham(this_model[4][i].x, this_model[4][i+1].x, this_model[4][i].y, this_model[4][i+1].y, model[4]);
	}*/

	//draw lines between the points using bresenham algorithm
	for(auto i = 0; i < converted_model[3].size(); i++)
	{
		bresenham(converted_model[3][i].x, converted_model[3][i+1].x, converted_model[3][i].y, converted_model[3][i+1].y, model[3]);
	}
	
	for(auto i = 0; i < converted_model[4].size(); i++)
	{
		bresenham(converted_model[4][i].x, converted_model[4][i+1].x, converted_model[4][i].y, converted_model[4][i+1].y, model[4]);
	}

	print_bitmap(model[3], 3);
	print_bitmap(model[4], 4);

	//compare pixel layers & load difference into a third layer
	//in this call, it will be blank because comparing model to itself will result in 0 differences
	compare_pixel_layers(model[3], model[4], model[5]);

	print_bitmap(model[5], 5);

	return 0;
}