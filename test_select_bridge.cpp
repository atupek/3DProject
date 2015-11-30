//test_seclect_bridge.cpp
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


	return 0;
}