#include "select_bridge.h"
#include <limits> //for inf
#include <algorithm>
using std::copy;
#include <cstdlib>
using std::size_t;
#include <iterator>
using std::distance;

//lmax calculates distance of heighest element being supported above the bridge
//input: set of segments intersecting sweep plane at given event
//output: highest z-value
double calculate_lmax(set<Anchoring_Segment> segments)
{
	double temp = 0.0;
	for(auto i = segments.begin(); i != segments.end(); i++)
	{
		if(i->intersect_pt.z > temp)
		{
			temp = i->intersect_pt.z;
		}
	}
	return temp;
}

//G(b) = (k-2)h(b)-w(b)
//w(b) = length of bridge
//h(b) = height of bridge
//k = # elements supported
double calculate_gain(double height, double length, int num_elements)
{
	return((num_elements-2)*height) - length;
}

//S(b) = G(b) - k * lmax(b)
//lmax(b) is the max length of structure connecting an element above to the bridge
double calculate_score(double gain, int num_elements, double lmax)
{
	return gain - num_elements * lmax;
}

void check_collision()
{
	//TODO
	//checking for collisions between bridge and model
}

vector<Anchoring_Segment> set_up_sort_segments_by_z(set<Anchoring_Segment> &segments)
{
	//copy segments into vector
	vector<Anchoring_Segment> unordered_segments;
	for(auto i = segments.begin(); i != segments.end(); i++)
	{
		unordered_segments.push_back(*i);
	}

	return unordered_segments;
}

void sort_segments_by_z(vector<Anchoring_Segment> &segment)
{
	//doing merge sort...
	merge_sort_z(segment.begin(), segment.end());
}

void stable_merge_z(vector<Anchoring_Segment>::iterator first, vector<Anchoring_Segment>::iterator middle, vector<Anchoring_Segment>::iterator last)
{
	vector<Anchoring_Segment> buffer(distance(first, last));

	auto in1 = first;
	auto in2 = middle;
	auto out = buffer.begin();

	while(in1 != middle && in2 != last)
	{
		if(in2->endpt1.z < in1->endpt1.z)
			*out++ = *in2++;
		else
			*out++ = *in1++;
	}

	copy(in1, middle, out);
	copy(in2, last, out);
	copy(buffer.begin(), buffer.end(), first);
}

void merge_sort_z(vector<Anchoring_Segment>::iterator first, vector<Anchoring_Segment>::iterator last)
{

	size_t size = distance(first, last);

	//base case
	if (size <=1)
		return;

	//recursive case
	auto middle = first;
	advance(middle, size/2);

	merge_sort_z(first, middle);
	merge_sort_z(middle, last);

	stable_merge_z(first, middle, last);
}

vector<Anchoring_Segment> set_up_sort_segments_by_y(set<Anchoring_Segment> &segments)
{
	//copy segments into vector
	vector<Anchoring_Segment> unordered_segments;
	for(auto i = segments.begin(); i != segments.end(); i++)
	{
		unordered_segments.push_back(*i);
	}

	return unordered_segments;
}

void sort_segments_by_y(vector<Anchoring_Segment> &segment)
{
	//doing merge sort...
	merge_sort_y(segment.begin(), segment.end());
}

void stable_merge_y(vector<Anchoring_Segment>::iterator first, vector<Anchoring_Segment>::iterator middle, vector<Anchoring_Segment>::iterator last)
{
	vector<Anchoring_Segment> buffer(distance(first, last));

	auto in1 = first;
	auto in2 = middle;
	auto out = buffer.begin();

	while(in1 != middle && in2 != last)
	{
		if(in2->endpt1.y < in1->endpt1.y)
			*out++ = *in2++;
		else
			*out++ = *in1++;
	}

	copy(in1, middle, out);
	copy(in2, last, out);
	copy(buffer.begin(), buffer.end(), first);
}

void merge_sort_y(vector<Anchoring_Segment>::iterator first, vector<Anchoring_Segment>::iterator last)
{

	size_t size = distance(first, last);

	//base case
	if (size <=1)
		return;

	//recursive case
	auto middle = first;
	advance(middle, size/2);

	merge_sort_y(first, middle);
	merge_sort_y(middle, last);

	stable_merge_y(first, middle, last);
}

double calc_dist(double x1, double y1, double x2, double y2)
{
	double dist;
	double x_squared = pow(x2 - x1, 2);
	//cout << "x2 - x1 = " << x2 - x1 << endl;
	//cout << "x_squared = " << x_squared << endl;
	double y_squared = pow(y2 - y1, 2);
	//cout << "y2 - y1 = " << y2 - y1 << endl;
	//cout << "y_squared = " << y_squared << endl;
	dist = sqrt(x_squared + y_squared);
	return dist;
}

//input to select_bridge set of segments (P) intersecting sweep plane at the current event
//returns bestBridge
//Container (C) initialized with segments that intesect sweep plane
//Z, sort all segments in container (C) by Z
//for each level (z)
//	for i=0 to size(Z)-1
//		Points_supported_by_bridge (A) <- 0 //initialize to empty set
//		for j=i to size(Z) -1
//			if distance from C[i] to C[j] <= max_distance
//				put C[j] into Points_supported_by_bridge
//				if there are collisions between points_supported_by_bridge, i, j, z then break
//				get bridge gain & score for points_supported_by_bridge, i, j, z
//				if gain>0 and score > bestScore then bestBridge = currentBridge
//return bestBridge
Bridge select_bridge(set<Anchoring_Segment> segments)
{
	/*//for debug
	cout << "SET BEFORE SORTING: " << endl;
	for(auto i = segments.begin(); i != segments.end(); i++)
	{
		i->print_coords(cout);
	}*/
	
	Bridge best_bridge;
	Bridge temp_bridge;

	//cout << "STARTING TEMP BRIDGE SUPPORTED POINTS SIZE: " << temp_bridge.supported_points.size() << endl;
	
	double max_distance = 30.0;
	double neg_inf(-std::numeric_limits<double>::infinity());
	double best_score = neg_inf;
	
	//sort segment intersections by y-coordinate
	vector<Anchoring_Segment> segments_by_y = set_up_sort_segments_by_y(segments);
	sort_segments_by_y(segments_by_y);

	/*//for debug
	cout << "VECTOR AFTER SORTING BY Y: " << endl;
	for(auto i = segments_by_y.begin(); i != segments_by_y.end(); i++)
	{
		i->print_coords(cout);
	}*/

	//put z-coordinate values into a set, sorted by increasing z
	set<double> z_set;
	for(auto i = segments_by_y.begin(); i != segments_by_y.end(); i++)
	{
		z_set.insert(i->height);
		//cout << "i->height: " << i->height << endl;
	}

	//for debug...just for testing...works
	/*
	for(auto i = 0; i < 11; i++)
	{
		if(i%2==0)
		{
			z_set.insert(-i * 3);
			cout << "Added into z: " << (-i * 3) << endl;
		}
		else
		{
			z_set.insert(i);
			cout << "Added into z: " << (i) << endl;
		}
	}*/
	/*
	//for debug...
	cout <<"Set z_set size: " << z_set.size() << endl;
	for(auto i = z_set.begin(); i != z_set.end(); i++)
	{
		cout << "Value in z: " << *i << endl;
	}*/

	for(auto k = z_set.begin(); k != z_set.end(); k++)
	{
		double this_z = *k;

		for(auto i = segments_by_y.begin(); i != segments_by_y.end(); i++)
		{
			temp_bridge.supported_points.clear(); //should be cleared out
			for(auto j = i; j != segments_by_y.end(); j++)
			{
				//compute distance between i & j intersect points
				//if less than max_distance, add intersect points into temp_bridge.supported_points
				double this_distance = calc_dist(i->intersect_pt.x, i->intersect_pt.y, j->intersect_pt.x, j->intersect_pt.y);
				//cout << "THIS DISTANCE: " << this_distance << endl;
				if(this_distance < max_distance)
				{
					temp_bridge.supported_points.insert(j->intersect_pt);

					/*//for debug...
					cout << "number of supported points: " << temp_bridge.supported_points.size() << endl;
					cout << "POINTS SUPPORTED BY TEMP BRIDGE: " << endl;
					for(auto m = temp_bridge.supported_points.begin(); m != temp_bridge.supported_points.end(); m++)
					{
						m->print_coords(cout);
					}*/

					temp_bridge.p1 = i-> intersect_pt;
					
					temp_bridge.p2 = j-> intersect_pt;
					
					double temp_gain = calculate_gain(this_z, this_distance, temp_bridge.supported_points.size());

					double l_max = calculate_lmax(segments);

					double temp_score = calculate_score(temp_gain, temp_bridge.supported_points.size(), l_max);

					if(temp_gain > 0.0 && temp_score > best_score)
					{
						best_bridge = temp_bridge;
						best_score = temp_score;
					}
				}
			}
		}
	}
	return best_bridge;
}

