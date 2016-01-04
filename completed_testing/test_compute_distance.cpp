#include "get_points/process_layers.h"
#include "get_points/point.h"
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;

vector<Point> point_vec;

void populate_point_vector()
{
	for(int i = 0; i <10; i++)
	{
		Point new_point(i, i*10, 0);
		point_vec.push_back(new_point);
	}
}

void call_calc_dist()
{
	for(auto i = point_vec.begin(); i < point_vec.end()-1; i++)
	{
		double dist = compute_distance(i->x, i->y, 0, 0);
		cout << "Distance between " << i->x << ", " << i->y << " and the origin is: " << dist << endl;
	}
}

int main()
{
	populate_point_vector();
	call_calc_dist();
	return 0;
}