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
//refactor print_bitmap into own .h & .cpp --DONE
//refactor the function calls to be only sending the layer to the the different functions, as opposed to sending vectors of layers
//figure out .5mm resolution as opposed to .1mm resolution, will assume the 'line' drawn by printer is .5mm wide -DONE
//this assumption is reasonable, considering that's what my extrusion width is set to in slicer
//also figure out how to make the lines drawn 'fatter' on the .1mm resolution...Dilate  --DONE for .5mm resolution
//create test data for the fatten lines function  --DONE

//Demo code for Monday:
// fill 2000 x 2000 vector with points from two subsequent layers  -DONE
// print bitmap for layer1 & layer 2  --DONE
// find binary difference between layers --DONE
// print bitmap of difference --DONE

//TODO next:
//compare compared_pixel_model's red pixels to processed_pixel_model's pixels
//and determine number of neighbors that red pixel has to determine whether or not it needs support
//if it has one(?) neighbor it doesn't need support.


#include "point.h"
#include "process_gcode.h"
#include "process_layers.h"
#include "process_pixels.h"
#include "bresenham.h"

//for printing bitmaps
#include "bitmap/bitmap_image.hpp"
#include "process_bitmap.h"

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
//400 for .5mm resolution, use multipy_by_two
//200 for 1.0mm resolution, no multiplication
int num_pixel_rows = 400;
int num_pixel_columns = 400;

model_pixels model;
model_pixels processed_pix_model;

//index will be off by one from processed_pix_model
//compared_pix_model[0] will be the difference between processed_pix_model[0] & [1]
//compared_pix_model[1] will be the difference between processed_pix_model[1] & [2]
//compared_pix_model[n] will be the difference between processed_pix_model[n] & [n+1]
model_pixels compared_pix_model;

//gets the file name
void get_file_name()
{
	cout << "Enter the name of the gcode file: ";
	cin.getline(gcodeFile, 256);
}

int main()
{
	//obvious.
	get_file_name();

	//match regex & create points, load them into layers, specified by the layer index
	all_layers this_model = match_regex(gcodeFile, model_layers, layer_index);

	//make a new model for the converted model
	all_layers converted_model = this_model;

	//multiply x, y, & e by 10 for the highest resolution (.1mm)
	//mulitply x, y, & e by 2 for the middle resolution (.5mm)
	//multiply by nothing for the lowest resolution (1mm)
	for(auto i = converted_model.begin(); i != converted_model.end(); i++)
	{
		multiply_by_two(*i);
	}

	//create vector of pixel layers, these are empty to begin with
	//and NEED TO BE POPULATED with fill_pixel_vector
	//takes a while for a 2000 x 2000 vector for 100 layers.  But, it works!
	for(int i = 0; i<layer_index; i++)
	{
		initialize_pixel_vector(model, num_pixel_rows, num_pixel_columns);
	}

	//create vector of pixel layers, empty to begin with
	//will be populated by fatten_lines
	//these are the pixel vectors that will eventually be compared
	//to get the points that need to be supported
	for(int i = 0; i<layer_index; i++)
	{
		initialize_pixel_vector(processed_pix_model, num_pixel_rows, num_pixel_columns);
	}

	//create vector of pixel layers, these are empty to begin with
	//and will be populated by compare_pixel_layers
	for(int i = 0; i < layer_index; i++)
	{
		initialize_pixel_vector(compared_pix_model, num_pixel_rows, num_pixel_columns);
	}

	//print x, y coordinates of Point vector
	//set to either just print the coordinates,
	//or to print all of the Point's data members in point.cpp
	//print_stuff(converted_model);
	
	//takes x, y coords from the first argument and sets those pixels in the second argument
	fill_pixel_vector(converted_model[3], model[3]);
	fill_pixel_vector(converted_model[4], model[4]);

	//for debug...appears to print correctly.
	print_bitmap(model[3], 0, num_pixel_rows, num_pixel_columns);
	
	//draw the lines in the pixel vector from point n to n+1
	//input is the point vector, output SHOULD BE the pixel vector (output is currently void)
	//and the lines in the pixel vector then need to be fattened up
	//PROBLEM: RIGHT NOW THE DRAW LINES DO NOT SAVE DATA IN A WAY I CAN WORK WITH IT!!!
	//I THINK THAT IS A PROBLEM, CHECKING THE BRESENHAM ALG THAT I WROTE TO SEE IF I AM WRONG OR RIGHT...

	//draw lines between the points using bresenham algorithm
	//this takes in a pixel layer and draws the line from n to n+1 for a pixel layer
	//this might work better than using the bitmap library...
	//CONFUSED: converted_model is a 2D vector of Points, so this_model[3] is a vector of Points
	//and the bresenham is supposed to take a pixel_layer which is a 2D vector of doubles...
	//Oh, wait.  the function call is right.  It's that I'm iterating through the vector of Points to
	//print out the stuff in the pixel layer...damn...confusing.
	//so when this is done, model[3] should have lines drawn in it...
	for(auto i = 0; i < converted_model[3].size()-1; i++)
	{
		bresenham(converted_model[3][i].x, converted_model[3][i+1].x, converted_model[3][i].y, converted_model[3][i+1].y, model[3]);
	}

	for(auto i = 0; i < converted_model[4].size()-1; i++)
	{
		bresenham(converted_model[4][i].x, converted_model[4][i+1].x, converted_model[4][i].y, converted_model[4][i+1].y, model[4]);
	}

	//fatten up the lines
	//takes first pixel layer and 'fattens the lines' into second pixel layer
	//input is pixel layer 1, output SHOULD BE pixel layer 2
	fatten_lines(model[3], processed_pix_model[3], num_pixel_rows, num_pixel_columns);

	//calling fatten_lines without calling bresenham to draw the lines, just to see the difference
	fatten_lines(model[4], processed_pix_model[4], num_pixel_rows, num_pixel_columns);

	//print the bitmap from the unfattened lines pixel layer for debug
	print_bitmap(model[3], 1, num_pixel_rows, num_pixel_columns);

	//print the bitmap from the fattened lines pixel layer for debug
	print_bitmap(processed_pix_model[3], 2, num_pixel_rows, num_pixel_columns);

	//processed_pix_model[4] is showing up as blank...
	//which is why the comparison is showing up as all green...ah-ha, because I didn't fill the pixel vector, I think...
	print_bitmap(processed_pix_model[4], 3, num_pixel_rows, num_pixel_columns);

	//compare pixel layers & load difference into a third layer
	compare_pixel_layers(processed_pix_model[3], processed_pix_model[4], compared_pix_model[3]);

	print_bitmap(compared_pix_model[3], 4, num_pixel_rows, num_pixel_columns);

	check_neighbors(compared_pix_model[3], processed_pix_model[3], num_pixel_rows, num_pixel_columns);

	print_bitmap(compared_pix_model[3], 5, num_pixel_rows, num_pixel_columns);

	return 0;
}