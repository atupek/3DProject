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
// so need a function to 'draw' that line in the sparse array
//TODO: Write Testing Data for 'printing out contents of pixel vector'  --DONE
//put pixel processing into own .cpp & .h
//put gcode processing into own .cpp & .h


#include "point.h"
#include "process_gcode.h"

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

//typedef vector<Point> this_layer;
//typedef vector<this_layer> all_layers; //vector<vector<Point>>


char gcodeFile[256];
all_layers model_layers;

int layer_index = 0; // not being properly incremented...


typedef vector<vector<double>> pixel_layer;
typedef vector<pixel_layer> model_pixels;
model_pixels model;

//compute distance
double compute_distance(double x1, double x2, double y1, double y2)
{
	double x_squared = pow(x2 - x1, 2);
	double y_squared = pow(y2 - y1, 2);
	return sqrt(x_squared + y_squared);
}

//gets the file name
void get_file_name()
{
	cout << "Enter the name of the gcode file: ";
	cin.getline(gcodeFile, 256);
}

//prints distance
void print_distance(this_layer layer)
{
	double this_distance = 0.0;
	for(int i = 1; i<layer.size(); i++)
	{
		this_distance = compute_distance(layer[i].x, layer[i-1].x, layer[i].y, layer[i-1].y);
		cout << "distance: " << this_distance << endl;
	}
}

//this prints out all the points
//should just print out the i->size()
//maybe I just need a function to iterate through the model_layers?
void print_stuff(all_layers &model)
{
	for(std::vector<this_layer>::iterator i = model.begin(); i != model.end(); i++)
	{
		for(std::vector<Point>::iterator j = i->begin(); j!= i->end(); j++)
			{
				//j->print_coords(cout);
				j->print_all(cout);
			}
		cout << "Number points in layer: " << i->size() << endl;
		//print_distance(*i);
		cout << "*********************END LAYER*****************************" << endl;
	}
}

void multiply_by_ten(this_layer &layer)
{
	for(auto i = layer.begin(); i != layer.end(); i++)
	{
		i->x *= 10;
		i->y *= 10;
		i->extrude_amt *= 10;
	}
}

//should probably only do this one layer at a time...
void initialize_pixel_vector()
{
	pixel_layer new_pix_layer;
	int num_rows = 2000;
	int num_columns = 2000;
	new_pix_layer.resize(num_rows);
	for(int i = 0; i<num_rows; i++)
	{
		new_pix_layer[i].resize(num_columns);
	}
	for(int j = 0; j<num_rows; j++)
	{
		for(int k = 0; k<num_columns; k++)
		{
			new_pix_layer[j][k] = 0.0;
		}
	}
	model.push_back(new_pix_layer);
}

//this_layer is vector of Points
//pixel_layer is 2D vector of doubles
//only sending one layer at a time
void fill_pixel_vector(this_layer &gcode_layer, pixel_layer &pix)
{
	for(auto i = gcode_layer.begin(); i != gcode_layer.end(); i++)
	{
		int x = lrint(i->x);
		int y = lrint(i->y);
		//cout << "x: " << x << ", y: " << y << endl;
		pix[x][y]=1.0; //if point is there, set it to 1.0
	}
}


void print_pixel_vector(pixel_layer & pix)
{

}

int main()
{

	get_file_name();
	all_layers this_model = match_regex(gcodeFile, model_layers, layer_index);


	//all_layers converted_model = model_layers;
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
		initialize_pixel_vector();
	}

	//print_stuff(converted_model);
	//print_stuff(model_layers);
	//print_stuff(this_model);

	
	//fill_pixel_vector(converted_model[3], model[3]);
	cout << "Num layers in model: " << model.size() << endl;

	return 0;
}