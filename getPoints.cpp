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
//DEBUG: get the extrusion amount properly printing in the print_all function call in the print_num_pts_in_layer function
//multiply x, y by ten & insert into 2D array 1 for filled w point, 0 for not filled
// draw line between points
//slope = m = (y2-y1)/(x2-x1)
//line (y-y1) = m * (x - x1)
// so need a function to 'draw' that line in the sparse array


#include "point.h"

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

typedef vector<Point> this_layer;
typedef vector<this_layer> all_layers; //vector<vector<Point>>


char gcodeFile[256];
all_layers model_layers;
int layer_index = 0;

//typedef double pixel_layer[200][200]; //create 3 pixel_layers using: pixel_layer p1, p2, p3;
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
	for(auto i = model.begin(); i != model.end(); i++)
	{
		for(auto j = i->begin(); j!= i->end(); j++)
			{
				j->print_coords(cout);
				//j->print_all(cout);
			}
		cout << "Number points in layer: " << i->size() << endl;
		print_distance(*i);
		cout << "*********************END LAYER*****************************" << endl;
	}
}

//refactor again? copied & pasted code for two functions...
//new function to convert string to double?
void get_perimeter_points(string line, this_layer &layer)
{
	istringstream iss1(line);
	vector<string> tokens1{istream_iterator<string>{iss1},
  		                  istream_iterator<string>{}};
    std::string::size_type sz;

    string x_coord_str = tokens1[1];
    x_coord_str.erase(x_coord_str.begin(), x_coord_str.begin()+1);
    double x_coord = stod(x_coord_str, &sz);
    
    string y_coord_str = tokens1[2];
    y_coord_str.erase(y_coord_str.begin(), y_coord_str.begin()+1);
    double y_coord = stod(y_coord_str, &sz);
    
    string extrude_amt_str = tokens1[3];
    extrude_amt_str.erase(extrude_amt_str.begin(), extrude_amt_str.begin()+1);
    double extrude_amt = stod(extrude_amt_str, &sz);

    Point new_point(x_coord, y_coord, extrude_amt, true);
    //cout << "New point: ";
    //new_point.print_all(cout);
    layer.push_back(new_point);
}

void get_infill_points(string line, this_layer & layer)
{
	istringstream iss1(line);
	vector<string> tokens1{istream_iterator<string>{iss1},
  		                  istream_iterator<string>{}};
    std::string::size_type sz;

    string x_coord_str = tokens1[1];
    x_coord_str.erase(x_coord_str.begin(), x_coord_str.begin()+1);
    double x_coord = stod(x_coord_str, &sz);
    
    string y_coord_str = tokens1[2];
    y_coord_str.erase(y_coord_str.begin(), y_coord_str.begin()+1);
    double y_coord = stod(y_coord_str, &sz);
    
    string extrude_amt_str = tokens1[3];
	extrude_amt_str.erase(extrude_amt_str.begin(), extrude_amt_str.begin()+1);
	double extrude_amt = stod(extrude_amt_str, &sz);

	Point new_point(x_coord, y_coord, extrude_amt, false);
    //cout << "New point: ";
    //new_point.print_all(cout);
    layer.push_back(new_point);
}

void make_new_layer(string line)
{
	this_layer new_layer;
	model_layers.push_back(new_layer);
	layer_index++;
   	//cout << "************** NEW LAYER *******************" << endl;
}

//parse through gcode file & match regexes
void match_regex()
{
	ifstream inFile(gcodeFile);
	string line;
	this_layer current_layer;
	model_layers.push_back(current_layer);
	regex expr1("G1 X");
	regex expr2("G1 Z"); 
	regex expr3("perimeter");
	regex expr4("infill");
	smatch match;

	while(getline(inFile, line, '\n'))
	{
		//look in the line for expr1 & expr2
		if(!regex_search(line, match, expr1) && !regex_search(line, match, expr2))
		{
			//do nothing if no match for expr1 && expr2
		}
		
		else
		{
			//if we match the first regex
			if(regex_search(line, match, expr1))
			{
				string new_line = match.suffix();
				if(regex_search(new_line, match, expr3))
				{
					//this is perimeter point
					get_perimeter_points(line, model_layers[layer_index]);
				}
				if(regex_search(new_line, match, expr4))
				{
					//this is infill point
					get_infill_points(line, model_layers[layer_index]);
				}
			}
			//if we match the second regex
			else if(regex_search (line,match,expr2))
			{
				make_new_layer(line);
			}

		}
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

void initialize_pixel_vector()
{
	pixel_layer new_pix_layer;
	int num_rows = 20;
	int num_columns = 20;
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

void fill_pixel_vector(all_layers &model)
{

}

int main()
{
	Point my_point(1.0, 4.3, false);

	my_point.print_coords(cout);
	my_point.print_all(cout);

	get_file_name();
	match_regex();
	//print_stuff(model_layers);

	all_layers converted_model = model_layers;

	for(auto i = converted_model.begin(); i != converted_model.end(); i++)
	{
		multiply_by_ten(*i);
	}
	//print_stuff(converted_model);
	//print_stuff(model_layers);
	
	//create vector of pixel layers
	//takes a while, so commented out.  But, it works!
	for(int i = 0; i<layer_index; i++)
	{
		initialize_pixel_vector();
	}
	cout << "Num layers in model: " << model.size() << endl;

	return 0;
}