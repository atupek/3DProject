//bresenham line algorithm

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <cstdlib> // for abs
#include <vector>
using std::vector;

#include "process_pixels.h"
#include "point.h"

pixel_layer test_layer;
model_pixels test_model;

vector<Point> test_point_vec;

int num_rows = 30;
int num_columns = 30;

void bresenham(int x1, int x2, int y1, int y2, pixel_layer &layer)
{
	int w = x2 - x1;
	int h = y2 - y1;
	int dx1 = 0;
	int dx2 = 0;
	int dy1 = 0;
	int dy2 = 0;

	//what about if w == 0 or h == 0???
	//looks like it still works...
	if (w < 0)
	{
		dx1 = -1;
		dx2 = -1;
	}
	else
	{
		dx1 = 1;
		dx2 = 1;
	}
	if (h < 0)
	{
		dy1 = -1;
	}
	else
	{
		dy1 = 1;
	}

	int longest = abs(w);
	int shortest = abs(h);

	if(!(longest > shortest))
	{
		longest = abs(h);
		shortest = abs(w);
		if(h < 0)
		{
			dy2 = -1;
		}
		else
		{
			dy2 = 1;
		}
		dx2 = 0;
	}

	int numerator = longest >> 1;
	for(int i = 0; i <= longest; i++)
	{
		//set x, y = 1.0;
		layer[x1][y1] = 1.0;
		numerator += shortest;
		if(! (numerator < longest))
		{
			numerator -= longest;
			x1 += dx1;
			y1 += dy1;
		}
		else
		{
			x1 += dx2;
			y1 += dy2;
		}
	}
}

void populate_test_model()
{
	test_layer.resize(num_rows);
	for(int i = 0; i<num_rows; i++)
	{
		test_layer[i].resize(num_columns);
	}
	/* //don't need to initialize to 0, is done automatically, yay, c++
	for(auto j = test_layer.begin(); j != test_layer.end(); j++)
	{
		for (auto k = j->begin(); k != j->end(); j++)
		{
			*k = 0.0;
		}
	}*/
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

	cout << test_point_vec.size() << endl;
	
	for(auto i = 0; i < test_point_vec.size()-1; i++)
	{
		bresenham(test_point_vec[i].x, test_point_vec[i+1].x, test_point_vec[i].y, test_point_vec[i+1].y, test_layer);
	}
	cout << "**********************************************" << endl;

	print_pixel_vector(test_layer);
	return 0;
}