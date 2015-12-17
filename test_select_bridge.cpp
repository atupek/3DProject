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

set<Anchoring_Segment> make_toy_example()
{
	cout << "making toy example" << endl;
	set<Anchoring_Segment> toy_set;

	Point point0(9, 9, 15, 0, 0);
	Point point1(19, 9, 15, 0, 0);
	Point point2(19, 9, 15, 0, 0);
	Anchoring_Segment segment0;
	segment0.endpt1 = point0;
	segment0.endpt2 = point1;
	segment0.intersect_pt = point2;
	segment0.height = point0.z;
	
	Point point3(19, 9, 15, 0, 0);
	Point point4(29, 9, 15, 0, 0);
	Point point5(19, 9, 15, 0, 0);
	Anchoring_Segment segment1;
	segment1.endpt1 = point3;
	segment1.endpt2 = point4;
	segment1.intersect_pt = point5;
	segment1.height = point3.z;

	Point point6(11, 15, 20, 0, 0);
	Point point7(26, 15, 20, 0, 0);
	Point point8(19, 15, 20, 0, 0);
	//Point point8(20, 15, 20, 0, 0);
	Anchoring_Segment segment2;
	segment2.endpt1 = point6;
	segment2.endpt2 = point7;
	segment2.intersect_pt = point8;
	segment2.height = point6.z;

	Point point9(15, 20, 18, 0, 0);
	Point point10(25, 20, 18, 0, 0);
	Point point11(19, 20, 18, 0, 0);
	//Point point11(21, 20, 18, 0, 0);
	Anchoring_Segment segment3;
	segment3.endpt1 = point9;
	segment3.endpt2 = point10;
	segment3.intersect_pt = point11;
	segment3.height = point9.z;

	Point point12(18, 24, 17, 0, 0);
	Point point13(29, 24, 17, 0, 0);
	Point point14(19, 24, 17, 0, 0);
	//Point point14(22, 24, 17, 0, 0);
	Anchoring_Segment segment4;
	segment4.endpt1 = point12;
	segment4.endpt2 = point13;
	segment4.intersect_pt = point14;
	segment4.height = point12.z;

	toy_set.insert(segment0);
	toy_set.insert(segment1);
	toy_set.insert(segment2);
	toy_set.insert(segment3);
	toy_set.insert(segment4);
	return toy_set;
}

int main()
{
	Bridge the_best_bridge;

	set<Anchoring_Segment> toy_segment_set = make_toy_example();
	the_best_bridge = select_bridge(toy_segment_set);
	cout << "Let's see what the toy set produces: " << endl;
	the_best_bridge.print_bridge_members(cout);
	for(auto i = the_best_bridge.supported_points.begin(); i != the_best_bridge.supported_points.end(); i++)
	{
		i->print_coords(cout);
	}

	return 0;
}