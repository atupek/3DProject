//test for anchoring_segment class
#include "anchoring_segment.h"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	Point test_point(100, 100, 0.8, 0.0, true);
	cout << "TEST POINT: ";
	test_point.print_coords_with_z(cout);
	double test_slope = 0.5;
	Anchoring_Segment test_anchor(test_point, test_slope, true);
	Anchoring_Segment test_anchor2(test_point, test_slope, false);
	test_anchor.print_coords(cout);
	test_anchor2.print_coords(cout);
	cout << test_anchor.intersected_points.size() << endl;
	return 0;
}