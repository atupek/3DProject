//process_gcode.cpp
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

//int layer_index = 0;

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
    double this_extrude_amt = stod(extrude_amt_str, &sz);

    Point new_point(x_coord, y_coord, this_extrude_amt, true);
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
	double this_extrude_amt = stod(extrude_amt_str, &sz);

	Point new_point(x_coord, y_coord, this_extrude_amt, false);
    //cout << "New point: ";
    //new_point.print_all(cout);
    layer.push_back(new_point);
}

void make_new_layer(string line, all_layers & model_layers, int &layer_index)
{
	this_layer new_layer;
	model_layers.push_back(new_layer);
	//layer_index++;
   	//cout << "************** NEW LAYER *******************" << endl;
}

//parse through gcode file & match regexes
all_layers match_regex(string gcodeFile, all_layers & model_layers, int &layer_index)
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
					//cout << "MATCHED PERIMETER POINT" << endl;
					get_perimeter_points(line, model_layers[layer_index]);
				}
				if(regex_search(new_line, match, expr4))
				{
					//this is infill point
					//cout << "MATCHED INFILL POINT" << endl;
					get_infill_points(line, model_layers[layer_index]);
				}
			}
			//if we match the second regex
			else if(regex_search (line,match,expr2))
			{
				//cout << "+++++++++++++++++++++GOT HERE+++++++++++++++++++++++++++" << endl;
				layer_index++;
				make_new_layer(line, model_layers, layer_index);
			}

		}
	}
	//cout << "layer_index: " << layer_index << endl;
	return model_layers;
}