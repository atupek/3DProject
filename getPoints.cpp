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
//just got layers sepearated Wahoo!
//Next thing TODO is get the different layers printing x, y -- DONE
//and the distances between subsequent points  -- DONE
//and get the number of items printed in each layer -- DONE
//TODO: REFACTOR
// and REWRITE REGEXES (need 3 now? 5?)
//GET ON GIT -- DONE
//TODO: Put perimeter lines in one container 'move to first perimeter point (no extrusion), perimeter (extrusion)' --DONE
//and put infill lines in another container 'move to first infill point (no extrusion), infill (extrusion)' --DONE
//and ignore all other motions --DONE(?)
//multiply x, y by ten & insert into 2D array 1 for filled w point, 0 for not filled, line between points
//slope = m = (y2-y1)/(x2-x1)
//line (y-y1) = m * (x - x1)
// so need a function to 'draw' that line in the sparse array
//DAMN, for distance to work, I need to have the perimeters & infill in the same container...
//maybe throw everything also into one container?
//shit.  I think maybe I need to do some oop here.  grrr.

#include "point.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
#include <cstdlib>
#include <utility>
using std::pair;
using std::make_pair;
#include <string>
using std::string;
using std::stof;
#include <regex>
using std::regex;
using std::smatch;
using std::regex_search;
#include <vector>
using std::vector;
#include <sstream>
using std::istringstream;
using std::istream_iterator;
#include <math.h> //for sqrt

typedef pair<float, float> point;
typedef pair<point, float> line_data;
typedef vector<line_data> this_layer;
typedef vector<this_layer> all_layers;

char gcodeFile[256];
all_layers model_layers;
//all_layers model_perimeter_layers;
//all_layers model_infill_layers;
int layer_index = 0;

//this does what it's supposed to do
//can delete the debug, since print_distance exists elsewhere
float compute_distance(float x1, float x2, float y1, float y2)
{
	float x_squared = pow(x2 - x1, 2);
	float y_squared = pow(y2 - y1, 2);
	return sqrt(x_squared + y_squared);
}

//gets the file name
//this does what it's supposed to do
void get_file_name()
{
	cout << "Enter the name of the gcode file: ";
	cin.getline(gcodeFile, 256);
}

//prints x, y
//this does what it's supposed to do
void print_x_y(this_layer layer)
{
	for(auto i = layer.begin(); i!= layer.end(); i++)
	{
		cout << i->first.first << ", " << i->first.second << ", " << i->second << endl;
	}
}

//prints distance
//this does what it's supposed to do
void print_distance(this_layer layer)
{
	float this_distance = 0.0;
	for(int i = 1; i<layer.size(); i++)
	{
		this_distance = compute_distance(layer[i].first.first, layer[i-1].first.first, layer[i].first.second, layer[i-1].first.second);
		cout << "distance: " << this_distance << endl;
	}
}

//this prints out all the points, too because it calls the print_x_y function
//should just print out the i->size()
//maybe I just need a function to iterate through the model_layers?
void print_num_pts_in_layer()
{
	for(auto i = model_layers.begin(); i != model_layers.end(); i++)
	{
		cout << i->size() << endl;
		print_x_y(*i);
		print_distance(*i);
		cout << "*********************END LAYER*****************************" << endl;
	}
}

//rename to get points
//new function to convert string to float?
void get_points_from_line(string line, this_layer &layer)
{
	//cout << "getting points from line" << endl;
	//cout << "line here: " << line << endl;
	//this_layer current_layer = layer;
	istringstream iss1(line);
	//cout << "got here" << endl;
	//cout << "line: " << line << endl;
	vector<string> tokens1{istream_iterator<string>{iss1},
  		                  istream_iterator<string>{}};
  	//cout << "got to this next place" << endl;
    /*for(auto i=tokens1.begin(); i!=tokens1.end(); i++)
    {
    	cout << "in this place" << endl;
    	cout << *i << endl;
    }*/
    std::string::size_type sz;
    //cout << "got past sz" << endl;
    string x_coord_str = tokens1[1];
    //cout << "x coord string" << endl;
    x_coord_str.erase(x_coord_str.begin(), x_coord_str.begin()+1);
    float x_coord = stof(x_coord_str, &sz);
    string y_coord_str = tokens1[2];
    y_coord_str.erase(y_coord_str.begin(), y_coord_str.begin()+1);
    float y_coord = stof(y_coord_str, &sz);
    point this_point = make_pair(x_coord, y_coord);
    //cout << this_point.first << ", " << this_point.second << endl;
    string extrude_amt_str = tokens1[3];
    extrude_amt_str.erase(extrude_amt_str.begin(), extrude_amt_str.begin()+1);
    float extrude_amt = stof(extrude_amt_str, &sz);
    line_data this_data = make_pair(this_point, extrude_amt);
    //cout << this_data.first.first << ",\t" << this_data.first.second << ",\t" << this_data.second << endl;
    layer.push_back(this_data);
    //cout << layer.size() << endl;
    //current_layer.push_back(this_data);
    //cout << current_layer.size();
}

void make_new_layer(string line)
{
	//cout << "making new layer" << endl;
	//cout << current_layer.size();
	//cout << "2" << endl;
	//istringstream iss2(line);
	//vector<string> tokens2{istream_iterator<string>{iss2},
    //    istream_iterator<string>{}};
    /*for(auto i=tokens2.begin(); i!=tokens2.end(); i++)
    {
    	cout << *i << endl;
    }*/
	this_layer new_layer;
	model_layers.push_back(new_layer);
	layer_index++;
   	cout << "************** NEW LAYER *******************" << endl;
}

//rename function to match regex because that's what it does
void get_points()
{
	ifstream inFile(gcodeFile);
	string line;
	//int layer_index = 0;
	this_layer current_layer;
	model_layers.push_back(current_layer);
	regex expr1("G1 X");
	regex expr2("G1 Z"); 
	regex expr3("perimeter");
	regex expr4("infill");
	smatch match;
	//int num_lines = 0;
	//while(!inFile.eof()) //bad practice to use this
	while(getline(inFile, line, '\n'))
	{
		//this_layer current_layer; NOPE, ONLY ONE GETS PUT INTO THIS
		//get line from gcode file
			//getline(inFile, line);
		//okay, we can read in the file and spit it back out.
		//Yay.
		//cout << line << endl;
		//look in the line for expr1 & expr2
		if(!regex_search(line, match, expr1) && !regex_search(line, match, expr2))
		{
			//do nothing if no match for expr1 && expr2
			//cout << "working fine?" << endl;
		}
		
		else
		{
			//this_layer current_layer; //NOPE, ONLY ONE GETS PUT INTO THIS
			//if we match the first regex
			//this_layer layer;
			if(regex_search(line, match, expr1))
			{
				//cout << "line: " << line << endl;
				//this_layer layer;
				string new_line = match.suffix();
				//get_points_from_line(line, model_layers[layer_index]);
				if(regex_search(new_line, match, expr3))
				{
					//cout << "perimeter!" << endl;
					get_points_from_line(line, model_layers[layer_index]);
				}
				if(regex_search(new_line, match, expr4))
				{
					//cout << "infill!" << endl;
					get_points_from_line(line, model_layers[layer_index]);
				}
			}
			//if we match the second regex
			else if(regex_search (line,match,expr2))
			{
				//cout << "Z line Z: " << line << endl;
				/*this_layer new_layer;
				model_layers.push_back(new_layer);
				layer_index++;*/
				make_new_layer(line);
			}

		}
	}
	//cout << num_lines << endl; //okay, the num_lines increment works
}

int main()
{
	Point my_point(1.0, 4.3, false);
	//cout << my_point.x << endl;
	//cout << my_point.y << endl;
	//cout << my_point.perimeter << endl;

	my_point.print_coords(cout);
	my_point.print_all(cout);

	//get_file_name();
	//get_points();
	//print_num_pts_in_layer();
	//cout << compute_distance(0.0, 2.0, 0.0, 2.0) << endl;
	return 0;
}