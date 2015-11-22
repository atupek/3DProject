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
//so need a function to 'draw' that line in the sparse array
//TODO: Write Testing Data for 'printing out contents of pixel vector'  --DONE
//put pixel processing into own .cpp & .h --DONE
//put gcode processing into own .cpp & .h --DONE
//put layer processing into own .cpp & .h --DONE


#include "point.h"
#include "process_gcode.h"
#include "process_layers.h"
#include "process_pixels.h"

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

int layer_index = 0; // not being properly incremented...

model_pixels model;

//gets the file name
void get_file_name()
{
	cout << "Enter the name of the gcode file: ";
	cin.getline(gcodeFile, 256);
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
		initialize_pixel_vector(model);
	}

	print_stuff(converted_model);
	//print_stuff(model_layers);
	//print_stuff(this_model);

	
	//fill_pixel_vector(converted_model[3], model[3]);
	cout << "Num layers in model: " << model.size() << endl;

	return 0;
}