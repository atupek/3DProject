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

double pt_line_dist(Point p1, Point p2, double x0, double y0)
{
	double numerator = abs( (p2.y-p1.y)*x0 - (p2.x-p1.x)* y0 + (p2.x * p1.y) - (p2.y * p1.x) );
	double denominator = sqrt( pow((p2.y - p1.y), 2) + pow((p2.x - p1.x), 2));
	return numerator/denominator;
}

double calc_z_diff(Point supported_pt, double temp_bridge_height)
{
	return supported_pt.z - temp_bridge_height;
}

double calc_horiz_dist(Point p1, Point p2, Point supported_pt)
{
	return pt_line_dist(p1, p2, supported_pt.x, supported_pt.y);
}

//lmax calculates total distance to heighest element being supported above the bridge
//returns double
//find z-max for each point that is supported by the bridge
//calculates the horizontal distance between the two endpoint points (intersection pts)
//and the vertical distance between the bridge and z-max
//returns their sum
double calculate_lmax(Point p1, Point p2, set<Point> supported_pts)
{
	double z_max = 0;
	for(auto i = supported_pts.begin(); i != supported_pts.end(); i++)
	{
		if(i->z > z_max)
		{
			z_max = i->z;
		}
	}

	double horiz_max = 0;
	for(auto i = supported_pts.begin(); i != supported_pts.end(); i++)
	{
		double temp_dist = pt_line_dist(p1, p2, i->x, i->y);
		if(temp_dist > horiz_max)
		{
			horiz_max = temp_dist;
		}
	}

	//cout << "Calculating lmax: " << endl;
	double horizontal_distance = calc_dist(p1.x, p1.y, p2.x, p2.y);
	//cout << "Horizontal dist: " << horizontal_distance << endl;
	double total_horizontal_dist = horiz_max + horizontal_distance;
	//cout << "total horizontal dist: " << total_horizontal_dist << endl;
	double vertical_distance = z_max - p1.z;
	//cout << "Vertical dist: " << vertical_distance << endl;
	double lmax = total_horizontal_dist+ vertical_distance;
	return lmax;
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
double calculate_score(double gain, int num_elements, double lmax)
{
	return gain - (num_elements * lmax);
}

void check_collision()
{
	//TODO
	//checking for collisions between bridge and model
}

void stable_merge_y(vector<point_seg_pair>::iterator first, vector<point_seg_pair>::iterator middle, vector<point_seg_pair>::iterator last)
{
	vector<point_seg_pair> buffer(distance(first, last));

	auto in1 = first;
	auto in2 = middle;
	auto out = buffer.begin();

	while(in1 != middle && in2 != last)
	{
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
	vector<point_seg_pair> buffer(distance(first, last));

	auto in1 = first;
	auto in2 = middle;
	auto out = buffer.begin();

	while(in1 != middle && in2 != last)
	{
		if(in2->first.x < in1->first.x)
			*out++ = *in2++;
		else
			*out++ = *in1++;
	}

	copy(in1, middle, out);
	copy(in2, last, out);
	copy(buffer.begin(), buffer.end(), first);
}

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
	if(sweep_slope == std::numeric_limits<double>::infinity())
	{
		for(auto i = sweep_lines.begin(); i != sweep_lines.end(); i++)
		{
			//so now we're looking at an individual sweep line
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

Bridge select_bridge(vector<Sweep_line> & sweep_lines, double sweep_slope)
{
	Bridge best_bridge;
	double max_distance = 30.0;
	double max_horizontal = 5.0;//TODO:adjust these numbers! was 15
	double min_vertical_dist = 2.0;//TODO:adjust these numbers! was 0
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
		//cout << "*****************NEW SWEEP LINE**************************" << endl;
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
					
					//this comparison of x, y didn't seem to change anything.
					//if((temp_bridge.length < max_distance) && (temp_bridge.p1.x != temp_bridge.p2.x)
						//&& (temp_bridge.p1.y != temp_bridge.p2.y)) //make sure endpoints aren't the same, as opposed to making sure length != 0
					//the comparison of length >= 5 seems to work fine
					if((temp_bridge.length < max_distance) && (temp_bridge.length >= 5)) //!=0 because somehow pts are still being compared to selves
					{
						//cout << "LESS THAN MAX_DIST" << endl;
						//add m, n and any points between them to bridge's supported points vector
						for(int k = m; k < n+1; k++)
						{
							//cout << "adding point" << endl;
							//first check constraints for angled pillar...
							double temp_horiz_dist = calc_horiz_dist(temp_bridge.p1, temp_bridge.p2, i->intersected_points[k].second.endpt1);
							double temp_vert_dist = calc_z_diff(i->intersected_points[k].second.endpt1, temp_bridge.height);
							if((i->intersected_points[k].second.endpt1.z >= *j) //changing >= to > doesn't make a difference
									&& (temp_horiz_dist <= max_horizontal)
									&& (temp_vert_dist >= min_vertical_dist))
									//if(i->intersected_points[k].second.endpt1.z >= *j)
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
							//temp_bridge.print_bridge_members(cout);
							//cout << "temp gain: " << temp_gain << endl;
							double temp_lmax = calculate_lmax(temp_bridge.p1, temp_bridge.p2, temp_bridge.supported_points);
							//cout << "temp lmax: " << temp_lmax << endl;
							double temp_score = calculate_score(temp_gain, temp_bridge.supported_points.size(), temp_lmax);
							//cout << "********************temp score: " << temp_score << endl <<endl;
							temp_bridge.score = temp_score;
							//cout << "Temp bridge score: " << temp_bridge.score << endl;
							if(temp_score > best_score)
							{
								best_score = temp_score;
								best_bridge = temp_bridge;
								//cout << "New best bridge score: " << best_bridge.score << endl;
							}
						}
					}
				}
			}
		}
	}
	return best_bridge;
}


