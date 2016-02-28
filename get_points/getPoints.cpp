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
using std::ofstream;
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

//ofstream outFile("points_to_support.txt");

char gcodeFile[256];
//all the points from the original model
all_layers model_layers;

//all the points needing support
all_layers all_points_needing_support;

int layer_index = 0;

//2000 for 0.1mm resolution, use multiply_by_ten
//400 for .5mm resolution, use multipy_by_two
//200 for 1.0mm resolution, no multiplication
int num_pixel_rows = 400;
int num_pixel_columns = 400;

//initial model into which converted points go
//points only
model_pixels model;

//lines are drawn in this model
model_pixels processed_pix_model;

//lines are fattened in this model
model_pixels fattened_pix_model;

//index will be off by one from processed_pix_model
//compared_pix_model[0] will be the difference between processed_pix_model[0] & [1]
//compared_pix_model[1] will be the difference between processed_pix_model[1] & [2]
//compared_pix_model[n] will be the difference between processed_pix_model[n] & [n+1]
model_pixels compared_pix_model;

//final pixel model, points needing support
model_pixels final_pix_model;

//gets the file name
void get_file_name()
{
	cout << "Enter the name of the gcode file: ";
	cin.getline(gcodeFile, 256);
}

//for testing
//shifts points to the right 10mm
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

	cout << "Got file name...matching regex next..." << endl;
	//match regex & create points, load them into layers, specified by the layer index
	all_layers this_model = match_regex(gcodeFile, model_layers, layer_index);

	cout << "matched regexes...erasing first three layers next..." << endl;
	//first three layers of model have no points because of way gcode is sliced
	this_model.erase(this_model.begin(), this_model.begin()+2);
	//last layer must also be removed for some reason...
	layer_index--;

	cout << "erased first three layers...making new model..." << endl;
	//make a new model for the converted model
	all_layers converted_model = this_model;

	cout << "new model made...multiplying for resolution..." << endl;
	//multiply x, y, & e by 10 for the highest resolution (.1mm)
	//mulitply x, y, & e by 2 for the middle resolution (.5mm)
	//multiply by nothing for the lowest resolution (1mm)
	for(auto i = converted_model.begin(); i != converted_model.end(); i++)
	{
		multiply_by_two(*i);
	}

/*
	//testing z_height
	//points constructed okay
	for(auto i = converted_model.begin(); i != converted_model.end(); i++)
	{
		cout << "NEW LAYER: " << endl;
		for(auto j = i->begin(); j != i->end(); j++)
		{
			//j->print_coords(cout);
			j->print_coords_with_z(cout);
		}
	}*/

	//************************for testing only************************************** 
	//shiftPoints(converted_model[4]);

	cout << "new resolution done...initializing pixel vector..." << endl;
	//model
	//create vector of pixel layers, these are empty to begin with
	//and will be populated with fill_pixel_vector
	//using the points from converted_model
	for(int i = 0; i<layer_index; i++)
	{
		initialize_pixel_vector(model, num_pixel_rows, num_pixel_columns);
	}

	cout << "initializing second pixel vector..." << endl;
	//processed_pix_model
	//create vector of pixel layers, empty to begin with
	//POPULATED BY DRAW LINES FUNCTION (Bresenham)
	//TODO >>>> FIX THIS DOCUMENTATION NOTE
	//these are the pixel vectors that will be fattened up before being compared
	for(int i = 0; i<layer_index; i++)
	{
		initialize_pixel_vector(processed_pix_model, num_pixel_rows, num_pixel_columns);
	}

	cout << "initializing third pixel vector..." << endl;
	//fattened_pix_model
	//create vector of pixel layers, empty to begin with
	//POPULATED BY FATTEN LINES FUNCTION
	//these are the pixel vectors that will eventually be compared
	//(model[i+1], fattened_pix_model[i])
	//to get the points that need to be supported
	for(int i = 0; i<layer_index; i++)
	{
		initialize_pixel_vector(fattened_pix_model, num_pixel_rows, num_pixel_columns);
	}

	cout << "initializing fourth pixel vector..." << endl;
	//compared_pix_model
	//create vector of pixel layers, these are empty to begin with
	//POPULATED BY COMPARE_PIXEL_LAYERS FUNCTION
	//comparison between model[i+1] and fattened_pix_model[i]
	//these points are then used in final check neighbors
	for(int i = 0; i < layer_index; i++)
	{
		initialize_pixel_vector(compared_pix_model, num_pixel_rows, num_pixel_columns);
	}

	cout << "initializng final pixel vector..." << endl;
	//final_pix_model
	//create vector of pixel layers, these are empty to begin with
	//POPULATED BY CHECK_NEIGHBORS FUNCTION
	//comparison between model[i+1] and fattened_pix_model[i+1]
	//to determine points that need support
	for(int i = 0; i < layer_index; i++)
	{
		initialize_pixel_vector(final_pix_model, num_pixel_rows, num_pixel_columns);
	}
	cout << "filling pixel vector..." << endl;
	//takes x, y coords from Point vector (first argument) and sets those pixels in the pixel layer (second argument)
	//loops through all Point vectors for the entire model
	for(auto i = 0; i < layer_index; i++)
	{
		fill_pixel_vector(converted_model[i], model[i]);
	}

/*
	//for debug...
	//before drawing lines...both should just be sets of points.
	print_bitmap(model[3], 0, num_pixel_rows, num_pixel_columns);
	print_bitmap(model[4], 1, num_pixel_rows, num_pixel_columns);*/

	cout << "drawing lines..." << endl;
	cout << "layer index: " << layer_index << endl;
	//draw lines between the points using bresenham algorithm
	//takes in a point_layer and draws a line from point n to n+1 in the corresponding pixel_layer in processed_pix_model
	for(auto i = 0; i < (layer_index-1); i++)
	{
		cout << "Processing layer: " << i << endl;
		//cout << "converted_model[i].size(): " << converted_model[i].size() << endl;
		for(auto j = 0; j < converted_model[i].size()-1; j++)
		{
			bresenham(converted_model[i][j].x, converted_model[i][j+1].x,
						converted_model[i][j].y, converted_model[i][j+1].y, processed_pix_model[i]);
		}
		cout << "layer processed..." << endl;
	}

	cout << "assigning procesed_pix_model to fattened_pix_model..." << endl;
	fattened_pix_model = processed_pix_model;
	/*
	//for debug...
	//after drawing lines, both should have lines.
	print_bitmap(processed_pix_model[3], 2, num_pixel_rows, num_pixel_columns);
	print_bitmap(processed_pix_model[4], 3, num_pixel_rows, num_pixel_columns);*/

	cout << "fattening the lines...." << endl;
	//fatten up the lines
	//takes pixel layer from processed_pix_model and fattens lines into corresponding layer of fattened_pix_model
	for(auto i = 0; i < layer_index; i++)
	{
		fatten_lines(processed_pix_model[i], fattened_pix_model[i], num_pixel_rows, num_pixel_columns);
	}

/*
	//for debug...
	//after fattening lines...	4, 5 should have thin lines.
	//							6, 7 should have fat lines
	print_bitmap(fattened_pix_model[3], 4, num_pixel_rows, num_pixel_columns);
	print_bitmap(fattened_pix_model[4], 5, num_pixel_rows, num_pixel_columns);*/

	cout << "comparing pixel layers..." << endl;
	//compare pixel layers & load difference into a third layer
	//compare pixel layers n (fattened_pix_model), n+1 (initial model)
	//and load difference into compared_pix_model
	for(auto i = 0; i < layer_index-1; i++)
	{
		compare_pixel_layers(fattened_pix_model[i], model[i+1], compared_pix_model[i]);
	}

	final_pix_model = compared_pix_model;

/*
	//for debug...
	//after comparing layers, they print okay
	print_bitmap(model[4], 6, num_pixel_rows, num_pixel_columns);
	print_bitmap(fattened_pix_model[3], 7, num_pixel_rows, num_pixel_columns);
	print_bitmap(compared_pix_model[3], 8, num_pixel_rows, num_pixel_columns);*/

	cout << "checking neighbors..." << endl;
	//check neighbors between model[i+1] and fattened_pix_model[i+1]
	//if neighbors exist in layer, then point in final_pix_model doesn't need support
	for(auto i = 0; i < layer_index-1; i++)
	{
		check_neighbors(compared_pix_model[i+1], fattened_pix_model[i+1], final_pix_model[i], num_pixel_rows, num_pixel_columns);
	}

/*
	//for debug...
	//after checking neighbors, prints okay
	print_bitmap(final_pix_model[3], 9, num_pixel_rows, num_pixel_columns);*/

	cout << "putting all points into vector..." << endl;
	//fill point vector with points that need support
	//all_points_needing_support is a vector of this_layer (vector<Point>)
	for(auto i = 0; i < final_pix_model.size(); i++)
	{
		this_layer points_needing_support;
		list_points(final_pix_model[i], points_needing_support, num_pixel_rows, num_pixel_columns, i);
		all_points_needing_support.push_back(points_needing_support);
	}

	/*
	//for debug, print out the points needing support
	for(auto i = all_points_needing_support.begin(); i != all_points_needing_support.end(); i++)
	{
		cout << "NEW LAYER: " << endl;
		for(auto j = i->begin(); j != i->end(); j++)
		{
			//j->print_coords(cout);
			j->print_coords_with_z(cout);
		}
	}*/
}

void points_to_file()
{
	//FILE * outFile;
   	//outFile = fopen ("outfile.txt","w");
   	ofstream outFile("points_to_support.txt");
	//outFile << "outfile" << endl;
	for(auto i = all_points_needing_support.begin(); i != all_points_needing_support.end(); i++)
	{
		for(auto j = i->begin(); j!= i->end(); j++)
		{
			//blah
			j->print_coords_with_z(outFile);
		}
	}
}

//for debug, print bmps of test data (from points shifted over)
void print_bmps()
{
	print_bitmap(model[3], 1, num_pixel_rows, num_pixel_columns);
	print_bitmap(processed_pix_model[3], 2, num_pixel_rows, num_pixel_columns);
	print_bitmap(fattened_pix_model[3], 3, num_pixel_rows, num_pixel_columns);
	print_bitmap(model[4], 4, num_pixel_rows, num_pixel_columns);
	print_bitmap(compared_pix_model[3], 5, num_pixel_rows, num_pixel_columns);
	print_bitmap(fattened_pix_model[4], 6, num_pixel_rows, num_pixel_columns);
	print_bitmap(final_pix_model[3], 7, num_pixel_rows, num_pixel_columns);
	print_bitmap(compared_pix_model[3], 8, num_pixel_rows, num_pixel_columns);
}

int main()
{
	getPoints();
	//print_bmps();
	points_to_file();

	return 0;
}