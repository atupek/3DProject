#include "select_bridge.h"
#include <limits> //for inf
#include <algorithm>
using std::copy;
#include <cstdlib>
using std::size_t;
#include <iterator>
using std::distance;


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

//S(b) = G(b) - k * lmax(b)
//lmax(b) is the max length of structure connecting an element above to the bridge
double calculate_score(double gain, int num_elements, double lmax)
{
	//0 - 4 * 3 = -12.0
	return gain - num_elements * lmax;
}

double calculate_distance(double x1, double y1, double x2, double y2)
{
	double x_squared = pow(x2 - x1, 2);
	double y_squared = pow(y2 - y1, 2);
	return sqrt(x_squared + y_squared);
}

void check_collision()
{
	//I HAVE NO IDEA HOW TO DO THIS
	//checking for collisions between bridge and model?

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
	merge_sort(segment.begin(), segment.end());
}

void stable_merge(vector<Anchoring_Segment>::iterator first, vector<Anchoring_Segment>::iterator middle, vector<Anchoring_Segment>::iterator last)
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

void merge_sort(vector<Anchoring_Segment>::iterator first, vector<Anchoring_Segment>::iterator last)
{

	size_t size = distance(first, last);

	//base case
	if (size <=1)
		return;

	//recursive case
	auto middle = first;
	advance(middle, size/2);

	merge_sort(first, middle);
	merge_sort(middle, last);

	stable_merge(first, middle, last);
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
	Bridge best_bridge;
	Bridge temp_bridge;
	double max_distance = 30.0;
	double neg_inf(-std::numeric_limits<double>::infinity());
	double best_score = neg_inf;
	//sort segment intersections by y-coordinate

	//sort segment intersections by z-coordinate
	//put z-coordinate values into a queue
	//double this_z = z_queue.top();
	//z_queue.pop();
	double this_z = 2.2; //just until z stuff is working...

	for(auto i = segments.begin(); i != segments.end()--; i++)
	{
		//temp_bridge.supported_points.clear(); //should be cleared out
		for(auto j = i; j != segments.end(); j++)
		{
			cout << "checking intersection points" << endl;
			//compute distance between i & j intersect points
			//if less than max_distance, add intersect poitns into temp_bridge.supported_points
			double this_distance = compute_distance(i->intersect_pt.x, i->intersect_pt.y, j->intersect_pt.x, j->intersect_pt.y);
			if(this_distance < max_distance)
			{
				cout << "less than max distance" << endl;
				temp_bridge.supported_points.insert(j->intersect_pt);
				temp_bridge.p1 = i-> intersect_pt;
				temp_bridge.p2 = j-> intersect_pt;
				double temp_gain = calculate_gain(this_z, this_distance, temp_bridge.supported_points.size());//double height, double length, int num_elements
				double l_max = calculate_lmax(this_z, this_distance);//double height, double length
				double temp_score = calculate_score(temp_gain, temp_bridge.supported_points.size(), l_max);//double gain, int num_elements, double lmax
				//if(temp_gain > 0.0 && temp_score > best_score)
				if(temp_score > best_score) //just until z stuff is working...
				{
					best_bridge = temp_bridge;
					cout << "best bridge replaced" << endl;
				}
			}
		}
	}
	return best_bridge;
}

