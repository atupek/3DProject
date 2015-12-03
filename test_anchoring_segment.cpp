//test for anchoring_segment class
#include "anchoring_segment.h"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	Point test_point(100, 100, 0, true);
	double test_slope = 0.5;
	Anchoring_Segment test_anchor(test_point, test_slope);
	test_anchor.print_coords(cout);
	return 0;
}