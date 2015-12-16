#include <iostream>
#include "anchoring_segment.h"
#include <set>
using std::set;
using std::cout;
using std::endl;
#include <vector>
using std::vector;

void create_points(int num_pts, vector<Point> & pt_vec)
{
	for(auto i = 0; i < num_pts; i++)
	{
		if(i%2==0)
		{
			Point new_pt(i, i, i*.2, 0, 0);
			pt_vec.push_back(new_pt);
		}
		else
		{
			Point new_pt(i, i, .4, 0, 0);
			pt_vec.push_back(new_pt);
		}
	}
}

int main()
{
	vector<Point> point_vec = {};
	int number_points = 10;
	create_points(number_points, point_vec);
	cout << "Size of point vec: " << point_vec.size() << endl;
	cout << "Members of Point vec: " << endl;
	for(auto i = point_vec.begin(); i != point_vec.end(); i++)
	{
		i->print_coords(cout);
	}

	set<Anchoring_Segment> segment_set;
	for(auto i = 0; i < point_vec.size(); i++)
	{
		Anchoring_Segment new_seg(point_vec[i], 1.0, false);
		Anchoring_Segment new_seg_1(point_vec[i], 1.0, true);
		segment_set.insert(new_seg);
		segment_set.insert(new_seg_1);
	}
	cout << "Size of Segment set: " << segment_set.size() << endl;
	for(auto i = segment_set.begin(); i != segment_set.end(); i++)
	{
		i->print_coords(cout);
	}

	//try to enter same points into set to see what happens
	//duplicating points, so < operator not working correctly...
	//let's try to fix it...
	for(auto i = 0; i < point_vec.size(); i++)
	{
		Anchoring_Segment new_seg(point_vec[i], 1.0, false);
		Anchoring_Segment new_seg_1(point_vec[i], 1.0, true);
		segment_set.insert(new_seg);
		segment_set.insert(new_seg_1);
	}
	cout << "Size of Segment set: " << segment_set.size() << endl;
	for(auto i = segment_set.begin(); i != segment_set.end(); i++)
	{
		i->print_coords(cout);
	}
	return 0;
}