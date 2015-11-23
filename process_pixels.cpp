#include "process_pixels.h"

//should probably only do this one layer at a time...
void initialize_pixel_vector(model_pixels & model, int num_rows, int num_columns)
{
	pixel_layer new_pix_layer;

	new_pix_layer.resize(num_rows);
	for(int i = 0; i<num_rows; i++)
	{
		new_pix_layer[i].resize(num_columns);
	}

/*
	for(int j = 0; j<num_rows; j++)
	{
		for(int k = 0; k<num_columns; k++)
		{
			new_pix_layer[j][k] = 0.0;
		}
	}*/

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

//print pixels as X's on screen, not needed now that
//bitmap library has been included
void print_pixel_vector(pixel_layer & pix)
{
	for(auto i = pix.begin(); i != pix.end(); i++)
	{
		for(auto j = i->begin(); j != i->end(); j++)
		{
			if(*j == 0.0)
			{
				cout << " "; //nothing if no point there
			}
			else
			{
				cout << "X"; //an X if there is a point
			}
		}
		cout << endl;
	}

}

//points in diff are the points that need to be supported
//TODO: make different value if upper layer has point or lower layer has point
//pix1 is layer n, pix2 is layer n+1, so pix1 is below pix2
//so pix1 & pix2 are both 2D vectors of doubles (1.0 or 0.0 depending on filled or not)
//they are of the same size...
void compare_pixel_layers(pixel_layer &pix1, pixel_layer &pix2, pixel_layer &diff)
{
	for(auto i = 0; i < pix1.size(); i++)
	{
		for (auto j = 0; j != pix1[i].size(); j++)
		{
			if(pix1[i][j] == pix2[i][j]) //if they're the same, then there shouldn't be a point
			{
				diff[i][j] = 0.0;
			}
			else if((pix1[i][j] != pix2[i][j]) && pix1[i][j] == 1.0)  //if pix1 has point there, but pix2 does not
			{
				diff[i][j] = 2.0; //if they're different, then there should be a point
			}
			else // if layers not equal and nothing under pix2
			{
				diff[i][j] = 1.0;
			}
		}
	}
}
