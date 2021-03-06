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

void initialize_gridded_pixel_vector(model_pixels & model, int num_rows, int num_columns)
{
	pixel_layer new_pix_layer;

	int decreased_num_rows = floor(num_rows/10)+1;
	int decreased_num_columns = floor(num_columns/10)+1;

	new_pix_layer.resize(decreased_num_rows);
	for(int i = 0; i<decreased_num_rows; i++)
	{
		new_pix_layer[i].resize(decreased_num_columns);
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
				diff[i][j] = 2.0; //nothing above it is okay, make it green
			}
			else // if layers not equal and nothing under pix2
			{
				diff[i][j] = 3.0; //nothing below it is not okay, make it red
			}
		}
	}
}

//not quite working right.  need to create test data & check it out...
void fatten_lines(pixel_layer & pix1, pixel_layer & pix2, int num_rows, int num_columns)
{
	int num_neighbors = 0;
	for(int i = 1; i < num_rows - 1; i++)
	{
		for (int j = 1; j < num_columns -1; j++)
		{
			//get_num_neighbors(i, j)
			if(pix1[i-1][j-1] == 1.0)
			{
				num_neighbors++;
			}
			if(pix1[i][j-1] == 1.0)
			{
				num_neighbors++;
			}
			if(pix1[i+1][j-1] == 1.0)
			{
				num_neighbors++;
			}
			if(pix1[i-1][j] == 1.0)
			{
				num_neighbors++;
			}
			if(pix1[i+1][j] == 1.0)
			{
				num_neighbors++;
			}
			if(pix1[i-1][j+1] == 1.0)
			{
				num_neighbors++;
			}
			if(pix1[i][j+1] == 1.0)
			{
				num_neighbors++;
			}
			if(pix1[i+1][j+1] == 1.0)
			{
				num_neighbors++;
			}
			if(num_neighbors >= 2)
			{
				//pix2[i][j] = 1.0;
				pix2[i-1][j-1] = 1.0;
				pix2[i][j-1] = 1.0;
				pix2[i+1][j-1] = 1.0;
				pix2[i-1][j] = 1.0;
				pix2[i+1][j] = 1.0;
				pix2[i-1][j+1] = 1.0;
				pix2[i][j+1] = 1.0;
				pix2[i+1][j+1] = 1.0;
			}
			if(pix1[i][j] == 1.0)
			{
				pix2[i][j] = 1.0;
			}
			num_neighbors = 0;
		}
	}
}

void check_neighbors(pixel_layer & diff_pix, pixel_layer & comp_pix, pixel_layer & final_pix, int num_rows, int num_columns)
{
	for(int i = 1; i < num_rows - 1; i++)
	{
		for (int j = 1; j < num_columns -1; j++)
		{
			if(diff_pix[i][j] == 3.0)//if diff_pix[i][j] == red we need to check number pixels
			{
				int num_neighbors = 0;  //figure out best place to put this, as it will need to be reset...
				if(comp_pix[i-1][j-1] == 1.0)
				{
					num_neighbors++;
				}
				if(comp_pix[i][j-1] == 1.0)
				{
					num_neighbors++;
				}
				if(comp_pix[i+1][j-1] == 1.0)
				{
					num_neighbors++;
				}
				if(comp_pix[i-1][j] == 1.0)
				{
					num_neighbors++;
				}
				if(comp_pix[i+1][j] == 1.0)
				{
					num_neighbors++;
				}
				if(comp_pix[i-1][j+1] == 1.0)
				{
					num_neighbors++;
				}
				if(comp_pix[i][j+1] == 1.0)
				{
					num_neighbors++;
				}
				if(comp_pix[i+1][j+1] == 1.0)
				{
					num_neighbors++;
				}
				if(num_neighbors >= 1)
				{
					//then we don't need to support this pixel
					final_pix[i][j] = 0.0;
				}
			}
		}
	}
}

void list_points(pixel_layer &diff_pix, this_layer & pts_to_support, int num_rows, int num_columns, int z_level)
{
	double this_z = (double)z_level * .2;
	for(int i = 1; i < num_rows - 1; i++)
	{
		for (int j = 1; j < num_columns -1; j++)
		{
			if(diff_pix[i][j] == 3.0)
			{
				//cout << "z_level: " << z_level << endl;
				//cout << "this z: "  << this_z << endl;
				Point new_point(i, j, this_z, 0.0, true);
				//Point new_point(i, j);
				pts_to_support.push_back(new_point);
				//cout << "New point added: ";
				//new_point.print_coords_with_z(cout);
			}
		}
	}
}


void drop_grid(pixel_layer & old_pts_to_support, pixel_layer & new_pts_to_support, int num_rows, int num_columns)
{
	//double this_z = (double)z_level * .2;
	for(int i = 1; i < num_rows -1; i++)
	{
		for(int j = 1; j < num_columns -1; j++)
		{
			if(old_pts_to_support[i][j] == 3.0) //if the point needs to be supported
			{
				new_pts_to_support[floor(i/10)][floor(j/10)]=3.0;
			}
		}
	}
}

void list_gridded_points(pixel_layer &gridded_pix, this_layer & pts_to_support, int num_rows, int num_columns, int z_level)
{
	int decreased_num_rows = floor(num_rows/10)+1;
	int decreased_num_columns = floor(num_columns/10)+1;

	double this_z = (double)z_level * .2;
	for(int i = 1; i < decreased_num_rows - 1; i++)
	{
		for (int j = 1; j < decreased_num_columns -1; j++)
		{
			if(gridded_pix[i][j] == 3.0)
			{
				//cout << "z_level: " << z_level << endl;
				//cout << "this z: "  << this_z << endl;
				Point new_point(i*10+5, j*10+5, this_z, 0.0, true);
				//Point new_point(i, j);
				pts_to_support.push_back(new_point);
				//cout << "New point added: ";
				//new_point.print_coords_with_z(cout);
			}
		}
	}
}

/*
void fix_offset(all_layers & pts_to_support)
{
	for(auto i = pts_to_support.begin(); i != pts_to_support.end(); i++)
	{
		for(auto j = i->begin(); j!= i->end(); j++)
		{
			//fix offset
			j->x -=200;
			j->y -=200;
		}
	}
}*/
