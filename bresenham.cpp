//bresenham line algorithm

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <cstdlib> // for abs

#include "process_pixels.h"

pixel_layer test_layer;
model_pixels test_model;

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
	test_layer[0][0] = 1.0;
	test_layer[5][5] = 1.0;
	test_layer[5][7] = 1.0;
	test_layer[9][7] = 1.0;
	test_layer[9][10] = 1.0;
	test_layer[11][15] = 1.0;
	test_layer[12][8] = 1.0;
	test_layer[16][16] = 1.0;
	test_layer[20][2] = 1.0;
	test_layer[29][29] = 1.0;
}

int main()
{
	//initialize_pixel_vector(test_model, num_rows, num_columns);
	populate_test_model();
	print_pixel_vector(test_layer);

	//I don't think this is right...
	for(int i = 0; i < num_rows-1; i++)
	{
		for (int j = 0; j < num_columns-1; j++)
		{

		}
	}
	bresenham(1, 0, 1, 0, test_layer);
	return 0;
}