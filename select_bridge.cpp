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
double calculate_lmax(set<Anchoring_Segment> segments)
{
	double temp = 0.0;
	//first iterate through segments
	for(auto i = segments.begin(); i != segments.end(); i++)
	{
		//then iterate through the segment's vector of intersected_points
		for(auto j = i->intersected_points.begin(); j != i->intersected_points.end(); j++)
		{
			if(j->z > temp)
			{
				temp = j->z;
			}
		}
	}
	return temp;
}

double new_lmax(Point p1, Point p2)
{
	double horizontal_distance = calc_dist(p1.x, p1.y, p2.x, p2.y);
	double vertical_distance = abs(p2.z - p1.z);
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
					temp_bridge.p1.z = *j;
					temp_bridge.p2.z = *j;
					//for debug
					//cout << endl;
					//temp_bridge.print_bridge_members(cout);
					//cout << endl;
					if((temp_bridge.length < max_distance) && (temp_bridge.length != 0)) //!=0 because somehow pts are still being compared to selves
					{
						//cout << "LESS THAN MAX_DIST" << endl;
						//add m, n and any points between them to bridge's supported points vector
						for(int k = m; k <= n; k++)
						{
							//cout << "adding point" << endl;
							if(i->intersected_points[k].second.endpt1.z > *j)
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
						cout << "height: " << *j << endl;
						cout << "bridge length: " << temp_bridge.length << endl;*/
						
						//and now we get gain, lmax, and score
						double temp_gain = calculate_gain(*j, temp_bridge.length, temp_bridge.supported_points.size());
						cout << "temp gain: " << temp_gain << endl;
						if(temp_gain > 0)
						{
							//temp_bridge.print_bridge_members(cout);
							//cout << "temp gain: " << temp_gain << endl;
							double temp_lmax = new_lmax(temp_bridge.p1, temp_bridge.p2);
							//cout << "temp lmax: " << temp_lmax << endl;
							double temp_score = calculate_score(temp_gain, temp_bridge.supported_points.size(), temp_lmax);
							//cout << "temp score: " << temp_score << endl <<endl;
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

//WORKING ON THIS --nope, abandoning for now, see above...
Bridge select_bridge_sweep_line(vector<Sweep_line> &sweep_lines, double sweep_slope)
{
	Bridge best_bridge;
	
	double max_distance = 30.0;
	double neg_inf(-std::numeric_limits<double>::infinity());
	double best_score = neg_inf;

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

	//go through each sweep_line and create potential bridges
	for(auto i = sweep_lines.begin(); i != sweep_lines.end(); i++)
	{
		cout << "******************************NEW Sweep Line: " << endl; //okay, only executes 4 times for example test which is correct
		Bridge temp_bridge;
		//i->print_sweep_line_members(cout);
		//for(auto j = i->intersected_points.begin(); j != i->intersected_points.end(); j++)
		for(int j = 0; j < (i->intersected_points.size())-1; j++) //TODO: TRYING WITH INDICES INSTEAD OF ITERATORS
		{
			cout << endl << "J INDEX: " << j << endl;
			//cout << "intersected_points.size: " << i->intersected_points.size() << endl;
			/*cout << "Point compare one: " << i->intersected_points[j].first.x << ", " << i->intersected_points[j].first.y << " at z= "
																							<< i->intersected_points[j].first.z << endl;*/
			//create temp_bridge with endpt of intersected point
			//and add associated anchoring segment's endpt1 to supported points (since endpt1 of segment will always be pt needing support)
			//Point new_endpt1(j->first);
			Point new_endpt1(i->intersected_points[j].first); //INDICES INSTEAD OF ITERATORS
			//Bridge temp_bridge;
			//temp_bridge.supported_points.clear();
			temp_bridge.p1 = new_endpt1;
			
			//for(auto k = i->intersected_points.begin(); k != i-> intersected_points.end(); k++)
			for(int k = j; k < i->intersected_points.size(); k++) //INDICES INSTEAD OF ITERATORS
			{
				cout << "K INDEX: " << k << endl;
				temp_bridge.supported_points.clear();
				for(auto m = z_set.begin(); m != z_set.end(); m++)
				{
					double this_z = *m;
					//cout << "this z: " << this_z << endl;
					temp_bridge.height = this_z;

					//Point new_endpt2(k->first);
					Point new_endpt2(i->intersected_points[k].first); //INDICES INSTEAD OF ITERATORS
					temp_bridge.p2 = new_endpt2;

					/*cout<< "From intersect point ";
					temp_bridge.p1.print_coords(cout);
					cout << "\tto intersect point ";
					temp_bridge.p2.print_coords(cout);
					cout << "\tat a height of z= " << this_z << endl;*/

					//if(j->second.endpt1.z >= this_z)
					//cout << "j's endpt1.z: " << i->intersected_points[j].second.endpt1.z << " >= this_z: " << this_z << endl;
					//cout << "k's endpt1.z: " << i->intersected_points[k].second.endpt1.z << " >= this_z: " << this_z << endl;

					//TODO: ISSUE IS SOMEWHERE WITH THE ADDING SUPPORTED POINTS...
					cout << "number of supported points before adds: " << temp_bridge.supported_points.size() << endl;
					if(i->intersected_points[j].second.endpt1.z >= this_z)
					{
						//cout << "true" << endl;
						//cout << "j's endpt1.z: " << i->intersected_points[j].second.endpt1.z << " >= this_z: " << this_z << endl;
						//temp_bridge.supported_points.insert(j->second.endpt1);
						temp_bridge.supported_points.insert(i->intersected_points[j].second.endpt1);
						cout << "number of supported points after add 1: " << temp_bridge.supported_points.size() << endl;
					}
					//if(k->second.endpt1.z >= this_z) //if endpt is above the current z-level, add it to supported points
					if(i->intersected_points[k].second.endpt1.z >= this_z)
					{
						//cout << "true" << endl;
						//cout << "k's endpt1.z: " << i->intersected_points[k].second.endpt1.z << " >= this_z: " << this_z << endl;
						//temp_bridge.supported_points.insert(k->second.endpt1);
						temp_bridge.supported_points.insert(i->intersected_points[k].second.endpt1);
						cout << "number of supported points after add 2: " << temp_bridge.supported_points.size() << endl;
					}

					//cout << "Check distance between temp bridge's end points..." << endl;
					double this_distance = calc_dist(temp_bridge.p1.x, temp_bridge.p1.y, temp_bridge.p2.x, temp_bridge.p2.y);
					temp_bridge.length = this_distance;
					//cout << this_distance << endl;

					//!= 0 because getting segfaults when trying to not compare intersected pts to themselves...
					if(this_distance < max_distance && this_distance != 0)
					{
						/*//for debug
						cout << "bridge is from:" << endl;
						cout << temp_bridge.p1.x << ", " << temp_bridge.p1.y << " to " << temp_bridge.p2.x << ", " << temp_bridge.p2.y << endl;
						cout << "number of supported points: " << temp_bridge.supported_points.size() << endl;
						cout << "at z = " << this_z << endl << endl;
						cout << "supported points: " << endl;
						for(auto n=temp_bridge.supported_points.begin(); n != temp_bridge.supported_points.end(); n++)
						{
							n->print_coords(cout);
						}*/
						//cout << "bridge to add" << endl;
						//calculate gain
						double temp_gain = calculate_gain(this_z, this_distance, temp_bridge.supported_points.size());
						//cout << "temp gain: " << temp_gain << endl;
						if(temp_gain > 0)
						{
							//calculate l_max and score
							double temp_l_max = new_lmax(temp_bridge.p1, temp_bridge.p2);
							//cout << "Lmax: " << temp_l_max << endl;
							double temp_score = calculate_score(temp_gain, temp_bridge.supported_points.size(), temp_l_max);
							//cout << "temp score: " << temp_score << endl;
							if(temp_score > best_score)
							{
								best_bridge = temp_bridge;
								best_score = temp_score;
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
Bridge select_bridge(set<Anchoring_Segment> &segments, double sweep_slope)
{
	/*//for debug
	cout << "SET BEFORE SORTING: " << endl;
	for(auto i = segments.begin(); i != segments.end(); i++)
	{
		i->print_coords(cout);
	}*/
	
	Bridge best_bridge;
	//Bridge temp_bridge; should declare temp_bridge later, I think

	//cout << "STARTING TEMP BRIDGE SUPPORTED POINTS SIZE: " << temp_bridge.supported_points.size() << endl;
	
	double max_distance = 30.0;
	double neg_inf(-std::numeric_limits<double>::infinity());
	double best_score = neg_inf;
	
	//sort segment intersections by y-coordinate
	vector<Anchoring_Segment> segments_by_y = set_up_sort_segments_by_y(segments);
	
	/*//for debug OKAY
	cout << "SEGMENTS BY Y SETUP:" << endl;
	for(auto i = segments_by_y.begin(); i != segments_by_y.end(); i++)
	{
		cout << "segment: ";
		i->print_coords(cout);
		i->print_intersect_pts(cout);
	}*/
	
	sort_segments_by_y(segments_by_y);

	//for debug OKAY
	/*cout << "VECTOR AFTER SORTING BY Y: " << endl;
	for(auto i = segments_by_y.begin(); i != segments_by_y.end(); i++)
	{
		i->print_coords(cout);
		//i->print_intersect_pts(cout);
	}*/

	//put z-coordinate values into a set, sorted by increasing z
	set<double> z_set;
	for(auto i = segments_by_y.begin(); i != segments_by_y.end(); i++)
	{
		z_set.insert(i->height);
		//cout << "i->height: " << i->height << endl;
	}

	/*//for debug STILL OKAY
	cout << "VECTOR AFTER GETTING Z Values: " << endl;
	for(auto i = segments_by_y.begin(); i != segments_by_y.end(); i++)
	{
		i->print_coords(cout);
		//i->print_intersect_pts(cout);
	}*/
	
	/*//for debug...OKAY
	cout <<"Set z_set size: " << z_set.size() << endl;
	for(auto i = z_set.begin(); i != z_set.end(); i++)
	{
		cout << "Value in z: " << *i << endl;
	}*/

	for(auto k = z_set.begin(); k != z_set.end(); k++)
	{
		/*//for debug OKAY
		cout << "VECTOR AT TOP OF Z - LOOP: " << endl;
		for(auto i = segments_by_y.begin(); i != segments_by_y.end(); i++)
		{
			i->print_coords(cout);
			//i->print_intersect_pts(cout);
		}*/

		double this_z = *k;
		//cout << "This z: " << *k << endl; //OKAY

		for(auto i = segments_by_y.begin(); i != segments_by_y.end(); i++) //removed -- from .end()
		{
			/*//for debug --WHY ARE WE LOSING THE NEGATIVE SEGMENTS? AND WHERE?!?! it had something to do with the -- or ++ so got rid of both
			cout << "********************CURRENT 'i' SEGMENT:";
			i->print_coords(cout);
			//i->print_intersect_pts(cout);
			cout << endl;*/

			for(auto j = i; j != segments_by_y.end(); j++) //removed i++
			{
				/*//for debug -- Getting the negative segments here
				cout << "CURRENT 'j' SEGMENT:";
				j->print_coords(cout);
				//j->print_intersect_pts(cout);
				cout << endl;*/
				
				//need to check to make sure segments' endpts are not equal, I think
				if(i->endpt1.x != j->endpt1.x && i->endpt1.y != j->endpt1.y)
				{
					//compute distance between i & j intersect points
					//if less than max_distance, add intersect points into temp_bridge.supported_points
					for(auto m = i->intersected_points.begin(); m != i->intersected_points.end(); m++)
					{
						//create a temp_bridge with m as its endpt 1 at height of this_z
						//TODO: really should write = operator for Point class at some time...
						Point new_p1(*m);
						Bridge temp_bridge;
						temp_bridge.p1 = new_p1;
						//and insert p1 into the points supported by the bridge
						temp_bridge.supported_points.insert(new_p1);
						//and set temp_bridge.length = 0
						temp_bridge.length = 0.0;

						/*//for debug
						cout << "************TEMP BRIDGE MEMBERS before adding p2: " << endl;
						temp_bridge.print_bridge_members(cout);*/

						for(auto n = j->intersected_points.begin(); n != j->intersected_points.end(); n++)
						{
							//double this_distance = calc_dist(i->intersect_pt.x, i->intersect_pt.y, j->intersect_pt.x, j->intersect_pt.y);
							double this_distance = calc_dist(m->x, m->y, n->x, n->y);

							/*//for debug
							cout << "*********************************************THIS DISTANCE: " << this_distance << endl;
							cout << "X1, Y1: " << m->x << ", " << m->y << endl;
							cout << "X2, Y2: " << n->.x << ", " << n->.y << endl;*/
							
							/*//for debug
							cout << "This distance, temp_bridge.length: " << endl;
							cout << this_distance << ", " << temp_bridge.length << endl;*/

							//check if on the same sweep line
							bool check_it = on_same_sweep_line(*m, *n, sweep_slope);
							//cout << "CHECK IT: " << check_it << endl;

							//check if this_distance < max_distance and points are on the same sweep line
							//if(this_distance < max_distance /*&& this_distance > temp_bridge.length*/)
							if((this_distance < max_distance) && check_it)
							{
								//cout << "IN HERE>>>>>>>>>" << endl;
								//if the distance is less than the max distance, make n the new p2 of temp_bridge
								Point new_p2(*n);
								temp_bridge.p2 = new_p2;
								//and insert p2 into the points supported by the bridge
								temp_bridge.supported_points.insert(new_p2);
								//and calculate temp_bridge's length
								temp_bridge.length = this_distance;
								//cout << "temp_bridge.length: " << temp_bridge.length << endl << endl;

								//for debug...
								/*cout << "number of supported points: " << temp_bridge.supported_points.size() << endl;
								cout << "POINTS SUPPORTED BY TEMP BRIDGE: " << endl;
								for(auto m = temp_bridge.supported_points.begin(); m != temp_bridge.supported_points.end(); m++)
								{
									m->print_coords(cout);
								}*/

								//for debug
								/*cout << "Sent to gain: " << endl;
								cout << "\tthis_z: " << this_z << endl;
								cout << "\tthis_distance: " << this_distance << endl;
								cout << "\tnumber of supported points: " << temp_bridge.supported_points.size() << endl;*/
								
								double temp_gain = calculate_gain(this_z, this_distance, temp_bridge.supported_points.size());
								//double l_max = calculate_lmax(segments);
								//double temp_score = calculate_score(temp_gain, temp_bridge.supported_points.size(), l_max);

								//for debug
								cout << "Temp Gain: " << temp_gain << endl;
								/*cout << "l_max: " << l_max << endl;
								cout << "Temp Score: " << temp_score << endl << endl;*/

								/*//for debug
								cout << "TEMP BRIDGE MEMBERS: " << endl;
								temp_bridge.print_bridge_members(cout);*/

								//this is the old code that I'm re-writing now...
								/*if(temp_gain > 0.0 && temp_score > best_score)
								{
									best_bridge = temp_bridge;
									best_score = temp_score;
								}*/
								if(temp_gain > 0)
								{
									//calculate l_max and score
									double temp_l_max = new_lmax(i->endpt1, j->endpt1);
									cout << "Lmax: " << temp_l_max << endl;
									double temp_score = calculate_score(temp_gain, temp_bridge.supported_points.size(), temp_l_max);
									cout << "temp score: " << temp_score << endl;
									if(temp_score > best_score)
									{
										best_bridge = temp_bridge;
										best_score = temp_score;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return best_bridge;
}

