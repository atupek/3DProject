//test for anchoring_segment class
#include "anchoring_segment.h"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	Point test_point(100, 100, 0.8);
	Point intersect_0(50, 50, 0.8);
	Point intersect_1(12, 5, 0.8);
	Point intersect_2(9, 12, 0.6);
	cout << "TEST POINT: ";
	test_point.print_coords_with_z(cout);
	double test_slope = 0.5;
	Anchoring_Segment test_anchor0(test_point, test_slope, true);
	Anchoring_Segment test_anchor1(test_point, test_slope, false);
	test_anchor0.print_coords(cout);
	test_anchor1.print_coords(cout);
	cout << "test_anchor0.intersected_points.size(): " << test_anchor0.intersected_points.size() << endl;
	test_anchor0.print_intersect_pts(cout);
	test_anchor0.intersected_points.push_back(intersect_0);
	test_anchor0.intersected_points.push_back(intersect_1);
	cout << "test_anchor0.intersected_points.size() after adding two: " << test_anchor0.intersected_points.size() << endl;
	test_anchor0.print_intersect_pts(cout);
	return 0;
}