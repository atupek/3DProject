//getPoints.cpp
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

//initial model into which converted points go
model_pixels model;

//lines are drawn in this model
model_pixels processed_pix_model;

//index will be off by one from processed_pix_model
//compared_pix_model[0] will be the difference between processed_pix_model[0] & [1]
//compared_pix_model[1] will be the difference between processed_pix_model[1] & [2]
//compared_pix_model[n] will be the difference between processed_pix_model[n] & [n+1]
model_pixels compared_pix_model;

//these are the points that should be sent to the next algorithm that creates the bridges
this_layer points_needing_support;

//gets the file name
void get_file_name()
{
	cout << "Enter the name of the gcode file: ";
	cin.getline(gcodeFile, 256);
}

//for testing points
//shift to the right 10mm
//takes a vector of Points
//and adds 10 to the x coordinate
void shiftPoints(this_layer &pt_layer)
{
	for(auto i = 0; i < pt_layer.size(); i++)
	{
		pt_layer[i].x +=10.0;
	}
}

void getPoints()
{
	//obvious.
	get_file_name();

	//match regex & create points, load them into layers, specified by the layer index
	all_layers this_model = match_regex(gcodeFile, model_layers, layer_index);

	//first three layers of model have no points because of way gcode is sliced
	this_model.erase(this_model.begin(), this_model.begin()+2);
	//last layer must also be removed for some reason...
	layer_index--;

	//make a new model for the converted model
	all_layers converted_model = this_model;

	//multiply x, y, & e by 10 for the highest resolution (.1mm)
	//mulitply x, y, & e by 2 for the middle resolution (.5mm)
	//multiply by nothing for the lowest resolution (1mm)
	for(auto i = converted_model.begin(); i != converted_model.end(); i++)
	{
		multiply_by_two(*i);
	}

	//for testing
	shiftPoints(converted_model[4]);

	//create vector of pixel layers, these are empty to begin with
	//and will be populated with fill_pixel_vector
	//using the points from converted_model
	for(int i = 0; i<layer_index; i++)
	{
		initialize_pixel_vector(model, num_pixel_rows, num_pixel_columns);
	}

	//create vector of pixel layers, empty to begin with
	//will be populated by fatten_lines
	//these are the pixel vectors that will eventually be compared
	//(model layer n+1, processed_pix_model layer n+1)
	//to get the points that need to be supported
	for(int i = 0; i<layer_index; i++)
	{
		initialize_pixel_vector(processed_pix_model, num_pixel_rows, num_pixel_columns);
	}

	//create vector of pixel layers, these are empty to begin with
	//and will be populated by compare_pixel_layers
	//and then be compared with processed_pix_model layer n-1
	//to determine points that need support
	for(int i = 0; i < layer_index; i++)
	{
		initialize_pixel_vector(compared_pix_model, num_pixel_rows, num_pixel_columns);
	}
	
	//takes x, y coords from Point vector (first argument) and sets those pixels in the pixel layer (second argument)
	//loops through all Point vectors for the entire model
	for(auto i = 0; i < layer_index; i++)
	{
		fill_pixel_vector(converted_model[i], model[i]);
	}

	//for debug...
	//before drawing lines...both should just be sets of points.
	//OK, THEY PRINT CORRECTLY
	print_bitmap(model[3], 0, num_pixel_rows, num_pixel_columns);
	print_bitmap(model[4], 1, num_pixel_rows, num_pixel_columns);

	//draw lines between the points using bresenham algorithm
	//takes in a point_layer and draws a line from point n to n+1 in the corresponding pixel_layer in the initial model
	for(auto i = 0; i < layer_index; i++)
	{
		cout << "converted_model[i].size(): " << converted_model[i].size() << endl;
		for(auto j = 0; j < converted_model[i].size()-1; j++)
		{
			bresenham(converted_model[i][j].x, converted_model[i][j+1].x, converted_model[i][j].y, converted_model[i][j+1].y, model[i]);
		}
	}

	//for debug...
	//after drawing lines...both should have lines.
	//OKAY, THEY PRINT CORRECTLY
	print_bitmap(model[3], 2, num_pixel_rows, num_pixel_columns);
	print_bitmap(model[4], 3, num_pixel_rows, num_pixel_columns);

	//fatten up the lines
	//takes pixel layer from initial model and fattens lines into corresponding layer of processed_pix_model
	for(auto i = 0; i < layer_index; i++)
	{
		fatten_lines(model[i], processed_pix_model[i], num_pixel_rows, num_pixel_columns);
	}

	//for debug...
	//after fattening lines...	4, 5 should have thin lines.
	//							6, 7 should have fat lines
	//THEY PRINT OKAY, BUT FATTEN LINES NEEDS TO BE TWEAKED *********************TODO*******************************
	print_bitmap(model[3], 4, num_pixel_rows, num_pixel_columns);
	print_bitmap(model[4], 5, num_pixel_rows, num_pixel_columns);
	print_bitmap(processed_pix_model[3], 6, num_pixel_rows, num_pixel_columns);
	print_bitmap(processed_pix_model[4], 7, num_pixel_rows, num_pixel_columns);

	//compare pixel layers & load difference into a third layer
	//compare pixel layers n (processed_pix_model), n+1 (initial model)
	//and load difference into compared_pix_model
	for(auto i = 0; i < layer_index-1; i++)
	{
		compare_pixel_layers(processed_pix_model[i], model[i+1], compared_pix_model[i]);
	}

	//for debug...
	//after comparing layers...
	//THEY PRINT OKAY
	print_bitmap(model[4], 8, num_pixel_rows, num_pixel_columns);
	print_bitmap(processed_pix_model[3], 9, num_pixel_rows, num_pixel_columns);
	print_bitmap(compared_pix_model[3], 10, num_pixel_rows, num_pixel_columns);

	//check neighbors between processed_pix_model[n], compared_pix_model[n+1]
	//if neighbors exist in layer below, then point in compared_pix_model doesn't need support
	for(auto i = 0; i < layer_index-1; i++)
	{
		check_neighbors(compared_pix_model[i+1], processed_pix_model[i], num_pixel_rows, num_pixel_columns);
	}

	//for debug...
	//after checking neighbors...
	//PRINTS OKAY
	print_bitmap(compared_pix_model[3], 11, num_pixel_rows, num_pixel_columns);


	//fill point vector with points that need support
	//TODO: points_needing_support needs to be 2D vector of points, one vector of points for each layer
	list_points(compared_pix_model[3], points_needing_support, num_pixel_rows, num_pixel_columns);
/*
	for(auto i = points_needing_support.begin(); i != points_needing_support.end(); i++)
	{
		i->print_coords(cout);
	}*/
}

int main()
{
	getPoints();
	return 0;
}