//test fatten_lines

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <cstdlib> // for abs
#include <vector>
using std::vector;

#include "process_pixels.h"
#include "point.h"
#include "bresenham.h"


pixel_layer test_layer;
model_pixels test_model;

vector<Point> test_point_vec;

int num_rows = 30;
int num_columns = 30;

void populate_test_model()
{
	test_layer.resize(num_rows);
	for(int i = 0; i<num_rows; i++)
	{
		test_layer[i].resize(num_columns);
	}
}


//okay, this is some ugly hard-coding...
void populate_point_vec()
{
	Point p1(0.0, 0.0, 0.0, true);
	test_point_vec.push_back(p1);
	Point p2(5.0, 5.0, 0.0, true);
	test_point_vec.push_back(p2);
	Point p3(5.0, 7.0, 0.0, true);
	test_point_vec.push_back(p3);
	Point p4(9.0, 7.0, 0.0, true);
	test_point_vec.push_back(p4);
	Point p5(9.0, 10.0, 0.0, true);
	test_point_vec.push_back(p5);
	Point p6(11.0, 15.0, 0.0, true);
	test_point_vec.push_back(p6);
	Point p7(12.0, 8.0, 0.0, true);
	test_point_vec.push_back(p7);
	Point p8(16.0, 16.0, 0.0, true);
	test_point_vec.push_back(p8);
	Point p9(20.0, 2.0, 0.0, true);
	test_point_vec.push_back(p9);
	Point p10(29.0, 29.0, 0.0, true);
	test_point_vec.push_back(p10);
}


int main()
{
	populate_test_model();
	print_pixel_vector(test_layer);
	populate_point_vec();

	//need to fill pixel vector with points from test_point_vec
	fill_pixel_vector();

	cout << test_point_vec.size() << endl;
	
	cout << "**********************************************" << endl;

	print_pixel_vector(test_layer);
	return 0;
}