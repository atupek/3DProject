#include "select_bridge.h"
#include <limits> //for inf
#include <algorithm>
using std::copy;
#include <cstdlib>
using std::size_t;
#include <iterator>
using std::distance;
#include <cmath> //for abs
using std::abs;

//lmax calculates distance of heighest element being supported above the bridge
//input: set of segments intersecting sweep plane at given event
//output: highest z-value
//NOPE ********************************TODO**********************************
//returns double
//for each point that is supported by the bridge, calculate the lmax
//and return the largest value calculated
//calculates the horizontal distance between the two points (the intersection pt and the point supported)
//and the vertical distance between the two points
//if that number is greater than the given max, then it becomes the new max
double new_calc_lmax(Point p1, Point p2, set<Point> supported_pts)
{
	double z_max = 0;
	for(auto i = supported_pts.begin(); i != supported_pts.end(); i++)
	{
		if(i->z > z_max)
		{
			z_max = i->z;
		}
	}
	//cout << "Calculating lmax: " << endl;
	double horizontal_distance = calc_dist(p1.x, p1.y, p2.x, p2.y);
	//cout << "Horizontal dist: " << horizontal_distance << endl;
	double vertical_distance = z_max - p1.z;
	//cout << "Vertical dist: " << vertical_distance << endl;
	double lmax = horizontal_distance + vertical_distance;
	return lmax;
}

//G(b) = (k-2)h(b)-w(b)
//w(b) = length of bridge
//h(b) = height of bridge
//k = # elements supported
//*************TODO******************CHECK: as long as everything sent is correct, will return correct
double calculate_gain(double height, double length, int num_elements)
{
	//cout << "GAIN: " << ((num_elements-2)*height) - length << endl;
	return((num_elements-2)*height) - length;
}

//S(b) = G(b) - k * lmax(b)
//lmax(b) is the max length of structure connecting an element above to the bridge
//****************TODO************CHECK: as long as everything sent is correct, will return correct
double calculate_score(double gain, int num_elements, double lmax)
{
	return gain - (num_elements * lmax);
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
	//cout << "COPYING SEGMENTS INTO A VECTOR" << endl;
	vector<Anchoring_Segment> unordered_segments;
	for(auto i = segments.begin(); i != segments.end(); i++)
	{
		unordered_segments.push_back(*i);
		//cout << "Added segment: " << endl;
		//i->print_coords(cout);
		//i->print_intersect_pts(cout);
	}

	return unordered_segments;
}

void sort_segments_by_y(vector<Anchoring_Segment> &segment)
{
	//doing merge sort...
	//merge_sort_y(segment.begin(), segment.end());
}

void stable_merge_y(vector<point_seg_pair>::iterator first, vector<point_seg_pair>::iterator middle, vector<point_seg_pair>::iterator last)
{
	//vector<Anchoring_Segment> buffer(distance(first, last));
	vector<point_seg_pair> buffer(distance(first, last));

	auto in1 = first;
	auto in2 = middle;
	auto out = buffer.begin();

	while(in1 != middle && in2 != last)
	{
		//if(in2->endpt1.y < in1->endpt1.y)
			//*out++ = *in2++;
		if(in2->first.y < in1->first.y)
			*out++ = *in2++;
		else
			*out++ = *in1++;
	}

	copy(in1, middle, out);
	copy(in2, last, out);
	copy(buffer.begin(), buffer.end(), first);
}

void stable_merge_x(vector<point_seg_pair>::iterator first, vector<point_seg_pair>::iterator middle, vector<point_seg_pair>::iterator last)
{
	//vector<Anchoring_Segment> buffer(distance(first, last));
	vector<point_seg_pair> buffer(distance(first, last));

	auto in1 = first;
	auto in2 = middle;
	auto out = buffer.begin();

	while(in1 != middle && in2 != last)
	{
		//if(in2->endpt1.y < in1->endpt1.y)
			//*out++ = *in2++;
		if(in2->first.x < in1->first.x)
			*out++ = *in2++;
		else
			*out++ = *in1++;
	}

	copy(in1, middle, out);
	copy(in2, last, out);
	copy(buffer.begin(), buffer.end(), first);
}

//void merge_sort_y(vector<Anchoring_Segment>::iterator first, vector<Anchoring_Segment>::iterator last)
void merge_sort_y(vector<point_seg_pair>::iterator first, vector<point_seg_pair>::iterator last)
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

void merge_sort_x(vector<point_seg_pair>::iterator first, vector<point_seg_pair>::iterator last)
{

	size_t size = distance(first, last);

	//base case
	if (size <=1)
		return;

	//recursive case
	auto middle = first;
	advance(middle, size/2);

	merge_sort_x(first, middle);
	merge_sort_x(middle, last);

	stable_merge_x(first, middle, last);
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

//to check if both points are on same sweep_line
//find slope between them, if slope == sweep_slope
//then they're on the same sweep_line
//if they are, then we continue with checking
//if they are not, then we skip
bool on_same_sweep_line(Point p1, Point p2, double sweep_slope)
{
	if(sweep_slope == 0)
	{
		//if horizontal, y1 == y2
		//cout << "slope == 0" << endl;
		//return (p1.y == p2.y);
		//hmmm...if sweep_slope is horizontal, then anch_slope is vertical
		//did I get my slopes messed up?
		//check to see if x's are equal...
		//cout << "p1.y: " << p1.y << endl;
		//cout << "p2.y: " << p2.y << endl;
		if(p1.y == p2.y)
		{
			//cout << "P1, P2: (" << p1.x << ", " << p1.y << "), (" << p2.x << ", " << p2.y << ")" << endl;
			return true;
		}
	}
	if(sweep_slope == std::numeric_limits<double>::infinity())
	{
		//if vertial, x1 == x2
		//cout << "slope == infinity" << endl;
		if(p1.x == p2.x)
		{
			//cout << "P1, P2: (" << p1.x << ", " << p1.y << "), (" << p2.x << ", " << p2.y << ")" << endl;
			return true;
		}
	}
	//make calculations for other cases and return true if on same line, otherwise return false
	double numerator = p1.y - p2.y;
	double denominator = p1.x - p2.x;
	double test_slope = numerator/denominator;
	if(sweep_slope == test_slope)
	{
		//cout << "slopes are equal" << endl;
		return true;
	}
	//cout << "OUT HERE THERE IS NO TRUTH" << endl;
	return false;
}

//sweep_line is a vector of pairs of points & anchoring segments
//pairs are typdef'ed as point_seg_pair
void sorted_by_y(Sweep_line &line_to_sort)
{
	merge_sort_y(line_to_sort.intersected_points.begin(), line_to_sort.intersected_points.end());
}

void sorted_by_x(Sweep_line & line_to_sort)
{
	merge_sort_x(line_to_sort.intersected_points.begin(), line_to_sort.intersected_points.end());
}

void sort_sweep_lines(vector<Sweep_line> & sweep_lines, double sweep_slope)
{
	//sort segment intersections by y-coordinate if sweep_slope == infinity
	//sort segment intersections by x_coordinate if sweep_slope == anything else
	//which will be a sort pairs by point's y-coord
	//vector<Sweep_line> sweep_lines_sorted_by_y = sort_by_y(sweep_lines);
	if(sweep_slope == std::numeric_limits<double>::infinity())
	{
		for(auto i = sweep_lines.begin(); i != sweep_lines.end(); i++)
		{
			//so now we're looking at an individual sweep line
			//sort_sweep_line_by_y
			sorted_by_y(*i);
		}

		/*//for debug
		cout << "Sweep slope = infinity" << endl;
		cout << "Should be sorted by y coords of intersected points" << endl;
		for(auto i = sweep_lines.begin(); i != sweep_lines.end(); i++)
		{
			i->print_sweep_line_members(cout);
		}*/

	}
	else
	{
		for(auto i = sweep_lines.begin(); i != sweep_lines.end(); i++)
		{
			//so now we're looking at an individual sweep line
			//sort_sweep_line_by_x
			sorted_by_x(*i);
		}

		/*//for debug
		cout << "Sweep slope = all others" << endl;
		cout << "Should be sorted by x coords of intersected points" << endl;
		for(auto i = sweep_lines.begin(); i != sweep_lines.end(); i++)
		{
			i->print_sweep_line_members(cout);
		}*/

	}
}

Bridge new_select_bridge(vector<Sweep_line> & sweep_lines, double sweep_slope)
{
	Bridge best_bridge;
	double max_distance = 30.0;
	double neg_inf(-std::numeric_limits<double>::infinity());
	double best_score = neg_inf;

	//sort intersection points on sweep lines
	sort_sweep_lines(sweep_lines, sweep_slope);

	//put z-coords into set
	//sorted by increasing z
	set<double> z_set;
	for(auto i = sweep_lines.begin(); i != sweep_lines.end(); i++)
	{
		for(auto j = i->intersected_points.begin(); j != i->intersected_points.end(); j++)
		{
			z_set.insert(j->first.z);
		}
	}
	/*//for debug:
	cout << "Size of z-set: " << z_set.size() << endl;*/

	//iterate through each sweep line
	for(auto i = sweep_lines.begin(); i != sweep_lines.end(); i++)
	{
		cout << "*****************NEW SWEEP LINE**************************" << endl;
		//do this for each z-level in the z-set
		for(auto j = z_set.begin(); j != z_set.end(); j++)
		{
			//sweep_line has vector of intersections, go through and for every pair of intersections (m, n)
			//check if points are within max_distance
			//if they are, add endpts at j's z-value (endp1 = m, endp2 =n)
			//check if points between (m, n inclusive) are above j's z-value
			//if they are, add points to bridge's supported points vector
			//get bridge's gain & score
			//then check pair of intersections (m+1, n)
			for(int m = 0; m < i->intersected_points.size()-1; m++)
			{
				//cout << "intersected point " << m << endl;
				for(int n = m+1; n < i-> intersected_points.size(); n++)
				{
					//cout << "inner intersected point " << n << endl;
					//reminder: .first is intersected_point,
					//			.second.endp1 is the anchoring segment's endpoint that needs supporting
					Bridge temp_bridge(i->intersected_points[m].first, i->intersected_points[n].first, *j);
					temp_bridge.p1.z = *j; //make sure bridge endpoints are at the proper z
					temp_bridge.p2.z = *j; //make sure bridge endpoints are at the proper z
					
					//for debug
					//cout << endl;
					//temp_bridge.print_bridge_members(cout);
					//cout << endl;
					
					if((temp_bridge.length < max_distance) && (temp_bridge.length != 0)) //!=0 because somehow pts are still being compared to selves
					{
						//cout << "LESS THAN MAX_DIST" << endl;
						//add m, n and any points between them to bridge's supported points vector
						for(int k = m; k < n+1; k++)
						{
							//cout << "adding point" << endl;
							if(i->intersected_points[k].second.endpt1.z >= *j)
							{
								//cout << "point above j" << endl;
								temp_bridge.supported_points.insert(i->intersected_points[k].second.endpt1);
								//cout << "added supported point" << endl;
							}
						}
						
						/*//for debug
						cout << "Bridge point 1: " << temp_bridge.p1.x << ", " << temp_bridge.p1.y << " at z: " << temp_bridge.p1.z << endl;
						cout << "Bridge point 2: " << temp_bridge.p2.x << ", " << temp_bridge.p2.y << " at z: " << temp_bridge.p2.z << endl;
						cout << "Bridge supported points size: " << temp_bridge.supported_points.size() << endl;
						cout << "\tSupported Points: " << endl;
						for(auto p = temp_bridge.supported_points.begin(); p != temp_bridge.supported_points.end(); p++)
						{
							cout << "\t";
							p->print_coords_with_z(cout);
						}
						cout << "height: " << *j << endl;
						cout << "bridge length: " << temp_bridge.length << endl;*/

						//and now we get gain, lmax, and score
						double temp_gain = calculate_gain(*j, temp_bridge.length, temp_bridge.supported_points.size());
						//cout << "temp gain: " << temp_gain << endl << endl;
						if(temp_gain > 0)
						{
							temp_bridge.print_bridge_members(cout);
							cout << "temp gain: " << temp_gain << endl;
							double temp_lmax = new_calc_lmax(temp_bridge.p1, temp_bridge.p2, temp_bridge.supported_points);
							cout << "temp lmax: " << temp_lmax << endl;
							double temp_score = calculate_score(temp_gain, temp_bridge.supported_points.size(), temp_lmax);
							cout << "temp score: " << temp_score << endl <<endl;
							if(temp_score > best_score)
							{
								best_score = temp_score;
								best_bridge = temp_bridge;
							}
						}
					}
				}
			}

		}
	}

	return best_bridge;
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


//NOTE: INTERSECTIONS & POINTS THAT NEED SUPPORT ARE NOT THE SAME THING
//BRIDGE CANNOT BE HIGHER THAN THE LOWEST Z OF A POINT ITS SUPPORTING

//for each anchoring segment with an intersection along the same sweep line
//check distance, if distance less than max_distance
//add Point (NOT INTERSECTION) supported to set of supported points
//calculate lmax which is total distance height + length to that point
//calculate gain & score
//where the length calculation in gain is from intersection to intersection along sweep line
//if gain > 0, calculate the lmax and score
//if score is better than best score, replace best bridge with temp bridge
