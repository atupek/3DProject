//test_seclect_bridge.cpp
//basic tester for select_bridge functions
#include "select_bridge.h"
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include "point.h"
#include "cube_primitive.h"
#include "pillar.h"
#include "event.h"

vector<Point> point_vec;
double test_slope = 0.5;


Point test_point(10, 10, 0, true);
Point test_point1(5, 5, 0, true);
Point test_point2(3, 3, 0, true);

Anchoring_Segment test_anchor(test_point, test_slope, true);
Anchoring_Segment test_anchor1(test_point1, test_slope, true);
Anchoring_Segment test_anchor2(test_point2, test_slope, true);

void make_points_vec(int _test_slope)
{
	for(int i = 0; i<10; i++)
	{
		Point new_point(i, i, 20.7, i+10, true);
		point_vec.push_back(new_point);
		//cout << "new point: ";
		//new_point.print_all(cout);
		if(i%2 == 0)
		{
			test_anchor2.intersected_points.push_back(new_point);
		}
		else
		{
			test_anchor1.intersected_points.push_back(new_point);
		}
	}
}

int main()
{
	double bridge_height = 1.0;
	double bridge_length = 2.0;
	int num_elements_supported = 4;
	double bridge_h1 = 2.0;
	double bridge_h2 = 4.0;

	set<Point> points_with_support;
/*
	double lmax = calculate_lmax(bridge_height, bridge_length);
	double gain = calculate_gain(bridge_height, bridge_length, num_elements_supported);
	double gain_above = calculate_gain_above_object(num_elements_supported, bridge_height, bridge_length, bridge_h1, bridge_h2);
	double score = calculate_score(gain, num_elements_supported, lmax);*/
/*
	cout << "lmax: " << lmax << " expected: 3.0" << endl;
	cout << "gain: " << gain << " expected: 0" << endl;
	cout << "gain_above: " << gain_above << " expected: -4.0" << endl;
	cout << "score: " << score << " expected: -12.0" << endl;*/

/*
	Point test_point(100, 100, 0, true);
	Point test_point1(50, 50, 0, true);
	Point test_point2(25, 0, 0, true);*/
	/*
	double test_slope = 0.5;
	Anchoring_Segment test_anchor(test_point, test_slope, true);
	Anchoring_Segment test_anchor1(test_point1, test_slope, true);
	Anchoring_Segment test_anchor2(test_point2, test_slope, true);*/
	//test_anchor.print_coords(cout);
	//cout << test_anchor.intersected_points.size() << endl;

	make_points_vec(test_slope);

	Point test_point3(75, 75, 0, true);
	double this_dist = calculate_distance(75, 75, 0, 0);
	//cout << "DISTANCE: " << this_dist << endl;

	double second_dist = compute_distance(75, 75, 0, 0);
	//cout << "SECOND DISTANCE: " << second_dist << endl;

	double x_squared = pow(0 - 75, 2);
	double y_squared = pow(0 - 75, 2);
	double final_distance = sqrt(x_squared + y_squared);
	//cout << "FINAL DISTANCE: " << final_distance << endl;

	test_anchor.intersected_points.push_back(test_point3);
	//cout << test_anchor.intersected_points.size() << endl;

	//set<Anchoring_Segment> test_set;
	//test_set.insert(test_anchor);
	//test_set.insert(test_anchor1);
	//test_set.insert(test_anchor2);
	vector<Anchoring_Segment> test_set;
	test_set.push_back(test_anchor);
	test_set.push_back(test_anchor1);
	test_set.push_back(test_anchor2);

/*
	cout << "Anchoring Segment test_set size: " << test_set.size() << endl;
	cout << "test_anchor size: "  << test_anchor.intersected_points.size() << endl;
	for(auto i = test_anchor.intersected_points.begin(); i != test_anchor.intersected_points.end(); i++)
	{
		cout << i->x << ", " << i->y << endl;
	}
	cout << "test_anchor1 size: " << test_anchor1.intersected_points.size() << endl;
	for(auto j = test_anchor1.intersected_points.begin(); j != test_anchor1.intersected_points.end(); j++)
	{
		cout << j->x << ", " << j->y << endl;
	}
	cout << "test_anchor2 size: " << test_anchor2.intersected_points.size() << endl;
	for(auto k = test_anchor2.intersected_points.begin(); k != test_anchor2.intersected_points.end(); k++)
	{
		cout << k->x << ", " << k->y << endl;
	}*/
	select_bridge(test_set, points_with_support);


	return 0;
}