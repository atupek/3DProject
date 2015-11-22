//process_layer.cpp
#include "process_layers.h"

//compute distance
double compute_distance(double x1, double x2, double y1, double y2)
{
	double x_squared = pow(x2 - x1, 2);
	double y_squared = pow(y2 - y1, 2);
	return sqrt(x_squared + y_squared);
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

void multiply_by_ten(this_layer &layer)
{
	for(auto i = layer.begin(); i != layer.end(); i++)
	{
		i->x *= 10;
		i->y *= 10;
		i->extrude_amt *= 10;
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
				//j->print_coords(cout);
				j->print_all(cout);
			}
		cout << "Number points in layer: " << i->size() << endl;
		print_distance(*i);
		cout << "*********************END LAYER*****************************" << endl;
	}
}

void compare_layers(this_layer &layer1, this_layer &layer2)
{
	
}