//test_seclect_bridge.cpp
//basic tester for select_bridge functions
#include "select_bridge.h"
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include "get_points/point.h"
//#include "cube_primitive.h"
//#include "pillar.h"
#include "event.h"
#include <limits> //for infinity

//NOTE:
//test set is written to only be viable for segment slope == 0
//any other slope will not work, because intersect points are slope-specific
double test_slope = 0.0;
//double test_slope = std::numeric_limits<double>::infinity();
//double test_slope = -1.0;
//double test_slope = -0.5;
//double test_slope = -0.3333;

set<Anchoring_Segment> make_toy_example_1()
{
	cout << "making toy example 1" << endl;
	set<Anchoring_Segment> toy_set_1;

	//segment 0
	Point point0(25, 20, 18);
	Point point1(55, 20, 18);
	Anchoring_Segment segment0;
	segment0.endpt1 = point0;
	segment0.endpt2 = point1;
	segment0.intersected_points = {point0};
	segment0.height = point0.z;
	segment0.slope = test_slope;

	//segment 1
	Point point2(19, 9, 15);
	Point point3(49, 9, 15);
	Point point4(25, 9, 15);
	Anchoring_Segment segment1;
	segment1.endpt1 = point2;
	segment1.endpt2 = point3;
	segment1.intersected_points = {point2, point4};
	segment1.height = point2.z;
	segment1.slope = test_slope;

	//segment2
	Point point5(18, 24, 17);
	Point point6(48, 24, 17);
	Anchoring_Segment segment2;
	segment2.endpt1 = point5;
	segment2.endpt2 = point6;
	Point point7(19, 24, 17);
	Point point8(25, 24, 17);
	segment2.intersected_points = {point5, point7, point8};
	segment2.height = point5.z;
	segment2.slope = test_slope;

	//segment3
	Point point9(11, 15, 20);
	Point point10(41, 15, 20);
	Anchoring_Segment segment3;
	segment3.endpt1 = point9;
	segment3.endpt2 = point10;
	Point point11(18, 15, 20);
	Point point12(19, 15, 20);
	Point point13(25, 15, 20);
	segment3.intersected_points = {point9, point11, point12, point13};
	segment3.height = point9.z;
	segment3.slope = test_slope;

	//segment4
	Point point14(-5, 20, 18);
	Anchoring_Segment segment4;
	segment4.endpt1 = point0;
	segment4.endpt2 = point14;
	Point point15(11, 20, 18);
	Point point16(18, 20, 18);
	Point point17(19, 20, 18);
	segment4.intersected_points = {point0, point15, point16, point17};
	segment4.height = point0.z;
	segment4.slope = test_slope;

	//segment5
	Point point18(-11, 9, 15);
	Anchoring_Segment segment5;
	segment5.endpt1 = point2;
	segment5.endpt2 = point18;
	Point point19(11, 9, 15);
	Point point20(18, 9, 15);
	segment5.intersected_points = {point2, point19, point20};
	segment5.height = point2.z;
	segment5.slope = test_slope;

	//segment6
	Point point21(-12, 24, 17);
	Anchoring_Segment segment6;
	segment6.endpt1 = point5;
	segment6.endpt2 = point21;
	Point point22(11, 24, 17);
	segment6.intersected_points = {point5, point22};
	segment6.height = point5.z;
	segment6.slope = test_slope;

	//segment7
	Point point23(-19, 15, 20);
	Anchoring_Segment segment7;
	segment7.endpt1 = point9;
	segment7.endpt2 = point23;
	segment7.intersected_points = {point9};
	segment7.height = point9.z;
	segment7.slope = test_slope;


	toy_set_1.insert(segment0);
	toy_set_1.insert(segment1);
	toy_set_1.insert(segment2);
	toy_set_1.insert(segment3);
	toy_set_1.insert(segment4);
	toy_set_1.insert(segment5);
	toy_set_1.insert(segment6);
	toy_set_1.insert(segment7);

	cout << "toy set made" << endl;

	return toy_set_1;
}

int main()
{
	Bridge the_best_bridge;

	set<Anchoring_Segment> toy_segment_set = make_toy_example_1();

	/*//for debug
	cout << "Toy Segment Set: " << endl;
	for(auto i = toy_segment_set.begin(); i != toy_segment_set.end(); i++)
	{
		i->print_coords(cout);
	}*/
	
	the_best_bridge = select_bridge(toy_segment_set);
	cout << "Let's see what the toy set produces: " << endl;
	the_best_bridge.print_bridge_members(cout);
	cout << endl;
	cout << "Point 1 with z: " << endl;
	the_best_bridge.p1.print_coords_with_z(cout);
	cout << endl;
	cout << "Point 2 with z: " << endl;
	the_best_bridge.p2.print_coords_with_z(cout);
	cout << endl;

	for(auto i = the_best_bridge.supported_points.begin(); i != the_best_bridge.supported_points.end(); i++)
	{
		i->print_coords_with_z(cout);
	}

/*  //for debug
	cout << "***************************Testing calc_dist************************" << endl;
	double dist0 = calc_dist(0, 0, 0, 5);
	cout << "dist 0: " << dist0 << " expected 5" << endl;
	double dist1 = calc_dist(0, 0, 5, 0);
	cout << "dist 1: " << dist1 << " expected 5" << endl;
	double dist2 = calc_dist(0, 5, 0, 0);
	cout << "dist 2: " << dist2 << " expected 5" << endl;
	double dist3 = calc_dist(5, 0, 0, 0);
	cout << "dist 3: " << dist3 << " expected 5" << endl;*/

	return 0;
}