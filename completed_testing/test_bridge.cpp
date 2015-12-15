//test_bridge.cpp
#include "bridge.h"
#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

int main()
{
	vector<Bridge> test_bridges;

	for(auto i = 0; i < 10; i++)
	{
		Point p1(i, i*10);
		Point p2(-i, i-10);
		Bridge new_bridge(p1, p2, i);
		test_bridges.push_back(new_bridge);
	}
	cout << "size of test_bridges: " << test_bridges.size() << endl;

	for(auto i = test_bridges.begin(); i != test_bridges.end(); i++)
	{
		i->print_bridge_members(cout);
	}

	return 0;
}