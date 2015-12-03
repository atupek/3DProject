//test for anchoring_segment class
#include "anchoring_segment.h"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	Point test_point(0, 0, 0, true);
	double test_slope = 1.0;
	Anchoring_Segment test_anchor(test_point, test_slope);
	return 0;
}