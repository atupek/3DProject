#include "select_bridge.h"

double calculate_lmax(double height, double length)
{
	//1.0 + 2.0 = 3.0
	return height + length;
}

//G(b) = (k-2)h(b)-w(b)
//w(b) = length of bridge
//h(b) = height of bridge
//k = # elements supported
double calculate_gain(double height, double length, int num_elements)
{
	//(4 - 2) * 1.0 - 2.0 = 0
	return((num_elements-2)*height) - length;
}

//in cases where bridge extremities are above the object, using free length of each vertical pillar instead of the bridge height
//h1 & h2 are the heights of the pillars before reaching the object.
double calculate_gain_above_object(int num_elements, double height, double length, double h1, double h2)
{
	//4 * 1.0 -2.0 -4.0 - 2.0 = -4.0
	return num_elements * height - h1 -h2 - length;
}

//S(b) = G(b) - k * lmax(b)
//lmax(b) is the max length of structure connecting an element above to the bridge
double calculate_score(double gain, int num_elements, double lmax)
{
	//0 - 4 * 3 = -12.0
	return gain - num_elements * lmax;
}

/*
	double bridge_height = 1.0;
	double bridge_length = 2.0;
	int num_elements_supported = 4;
	double bridge_h1 = 2.0;
	double bridge_h2 = 4.0;*/

