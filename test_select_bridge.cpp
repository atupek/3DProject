//test_seclect_bridge.cpp
//basic tester for select_bridge functions
#include "select_bridge.h"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	double bridge_height = 1.0;
	double bridge_length = 2.0;
	int num_elements_supported = 4;
	double bridge_h1 = 2.0;
	double bridge_h2 = 4.0;

	double lmax = calculate_lmax(bridge_height, bridge_length);
	double gain = calculate_gain(bridge_height, bridge_length, num_elements_supported);
	double gain_above = calculate_gain_above_object(num_elements_supported, bridge_height, bridge_length, bridge_h1, bridge_h2);
	double score = calculate_score(gain, num_elements_supported, lmax);

	cout << "lmax: " << lmax << " expected: 3.0" << endl;
	cout << "gain: " << gain << " expected: 0" << endl;
	cout << "gain_above: " << gain_above << " expected: -4.0" << endl;
	cout << "score: " << score << " expected: -12.0" << endl;

	Point test_point(100, 100, 0, true);
	double test_slope = 0.5;
	Anchoring_Segment test_anchor(test_point, test_slope);
	test_anchor.print_coords(cout);
	cout << test_anchor.intersected_points.size() << endl;

	set<Anchoring_Segment> test_set;
	test_set.insert(test_anchor);

	select_bridge(test_set);


	return 0;
}