//testPoint.cpp
//make some points & throw them in a vector, then call the print functions

#include "point.h"
#include <vector>
#include <iostream>
#include <queue>
using std::cout;
using std::endl;
using std::vector;
using std::queue;

vector<Point> point_vec;
queue<Point> point_q;

void make_points_vec()
{
	for(int i = 0; i<10; i++)
	{
		Point new_point(i, i, i+10, true);
		point_vec.push_back(new_point);
		cout << "new point: ";
		new_point.print_all(cout);
	}
}

void print_vec()
{
	cout << "Printing vec points: " << endl;
	for(auto i = point_vec.begin(); i != point_vec.end(); i++)
	{
		i->print_all(cout);
	}
}

void make_points_queue()
{
	for(int i = 0; i<10; i++)
	{
		Point new_point(i, i, i+10, true);
		point_q.emplace(new_point);
		cout << "new point in queue: ";
		new_point.print_all(cout);
	}

}

void print_q()
{
	cout << "Printing queue points: " << endl;
	while (!point_q.empty())
	{
		point_q.front().print_all(cout);
		point_q.pop();
	}
}

int main()
{
	make_points_vec();
	print_vec();

	//make_points_queue();
	//print_q();
	return 0;
}