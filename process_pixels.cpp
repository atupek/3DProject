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