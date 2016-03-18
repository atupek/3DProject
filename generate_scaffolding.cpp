//generate_scaffolding.cpp
//this is algorithm 2 from Bridging the Gap
#include "generate_scaffolding.h"
#include "select_bridge.h"
#include "bridge.h"
#include <limits> //for infinity
//#include "pillar.h"
//#include "cube_primitive.h"
#include <utility>
using std::pair;
using std::make_pair;
#include <fstream>
using std::ofstream;
#include <string>
using std::string;

ofstream out_file("scad_output.scad");

//creates anchoring segment
void draw_line(Point pt, double slope, set<Anchoring_Segment> &anchor_segments)
{
	//line should be orthogonal to the slope, of a specific given distance
	//endpoints are the point passed, and a point at the positive distance
	//endpoints are the point passed, and a point at the negative distance
	//thus, creates two anchoring segments
	//cout << "A LINE:" << endl;
	Anchoring_Segment new_segment(pt, slope, true);
	anchor_segments.insert(new_segment);

	/*//for debug
	cout << "TRUE SEGMENT: " << endl;
	cout << "true, new_x1: " << new_segment.new_x1 << endl;
	cout << "true, new_y1: " << new_segment.new_y1 << endl;
	new_segment.print_coords(cout);*/

	Anchoring_Segment new_segment1(pt, slope, false);
	anchor_segments.insert(new_segment1);

	/*//for debug
	cout << "FALSE SEGMENT: " << endl;
	cout << "false, new_x1: " << new_segment1.new_x1 << endl;
	cout << "false, new_y1: " << new_segment1.new_y1 << endl;
	new_segment1.print_coords(cout);*/
}

void create_anchoring_segments(set<Point> &point_set, set<Bridge> &bridge_set, set<Anchoring_Segment> &anchor_segments, vector<double> &sweep_direction, int i)
{
	double plane = sweep_direction[i];
	//for each Point in point_set, create anchor segment of length max_length orthoganl to sweep_direction, centered on Point
	for(auto j = point_set.begin(); j != point_set.end(); j++)
	{
		draw_line(*j, plane, anchor_segments);
	}
	for(auto j = bridge_set.begin(); j != bridge_set.end(); j++)
	{
		//Check if endpoint is open before creating anchoring segment
		if(j->pt1_open)
		{
			draw_line(j->p1, plane, anchor_segments);
		}
		if(j->pt2_open)
		{
			draw_line(j->p2, plane, anchor_segments);
		}
	}
}


void create_events(set<Anchoring_Segment> &_segments, set<Event> & events)
{
	for(auto i = _segments.begin(); i != _segments.end(); i++)
	{
		Event new_event(i->endpt1);
		new_event.event_segments.push_back(*i);
		auto set_it = events.find(new_event);
		if(set_it != events.end()) // if it's already in event set, add segment to its vector of segments
		{	
			//can't change items in a set, so have to create a new one
			Event changed_event(*set_it);
			
			//and then erase the old one
			events.erase(*set_it);
			
			//and add the segment to the new one
			changed_event.event_segments.push_back(*i);
			
			//and then insert the new one into the set
			events.insert(changed_event);
		}
		else // if it's not already in event set, then add it
		{
			events.insert(new_event);
		}
	}
}

//calculate intersections between anchoring segments and sweep plane
//slope1 is sweep plane slope
//slope2 is anchoring_segment slope
//if slope = 0, x-intersection DNE unless y = 0;  y = b
//if slope = inf, x-intersection = x; x = b
double calculate_x_intersection(double y_intercept1, double y_intercept2, double slope1, double slope2)
{
	if(slope1 == slope2) //check for parallel lines
	{
		return 0;
	}
	double numerator = y_intercept2 - y_intercept1;
	double denominator = slope1 - slope2;
	return numerator/denominator;
}

//if slope = 0, y-intersection = b; y = b
//if slope = inf, y-intersection DNE unless x = 0; x = b
double calculate_y_intersection(double y_intercept1, double y_intercept2, double slope1, double slope2)
{
	if(slope1 == slope2) //check for parallel lines
	{
		return 0;
	}
	double numerator = (y_intercept1 * slope2) - (y_intercept2 * slope1);
	double denominator = slope2 - slope1;
	return numerator/denominator;

}

//put intersected points into appropriate anchoring segment's intersected points vector
void find_intersections(set<Event> & events, vector<double> sweep_directions, int sweep_index, vector<Sweep_line> &sweep_line_vec)
{
	double sweep_slope = sweep_directions[sweep_index];
	//find y-intercept of line at point of event
	//then find y-intercept of anchoring segment line
	//then find intersection of both lines
	//if that intersection point is between the anchoring segment's endpoints
	//then add that intersection point to the anchoring segment's vector of intersection points
	for(auto i = events.begin(); i != events.end(); i++)
	{
		Sweep_line sweep_line;
		//for debug
		/*cout << "**************** I EVENT INFO:*********************" << endl;
		i->print_event_members(cout);*/

		//line 0
		double point_x = i->p1.x;
		double point_y = i->p1.y;
		double y_intercept = point_y - sweep_slope * point_x;
		//equation of line at the event point:
		// y = sweep_slope * x + y_intercept
		
		//for debug
		/*cout << "****************************EQUATION OF LINE: " << endl;
		cout << "y = " << sweep_slope << " * x + " << y_intercept << endl;*/

		//now go through each anchoring segment in the set of events and find out if they intersect
		for(auto j = events.begin(); j != events.end(); j++)
		{
			/*//for debug:
			cout << "J EVENT INFO:" << endl;
			j->print_event_members(cout);*/
			
			for(auto k = j->event_segments.begin(); k != j->event_segments.end(); k++)
			{
				//line 1
				double segment_x = k->endpt1.x;
				double segment_y = k->endpt1.y;
				double segment_slope = k->slope;
				double segment_y_intercept = segment_y - segment_slope * segment_x;

				double x_int;
				double y_int;

				//check if segment_slope is 0, if it is, then sweep_slope is infinity
				if(segment_slope == 0) //check for horizontal line
				{
					x_int = point_x;
					y_int = segment_y;
					
					/*//for debug
					cout << "Intersection at: " << x_int << ", " << y_int << endl;*/

					//(x_int, y_int) is the intersection between the sweep slope at that point and the anchoring segments
					//check that it is within the endpoints of the anchoring segment
					//first need to check if endpt1 < endpt2
					
					/*//for debug:
					cout << "CHECKING ENDPOINTS: " << endl;
					cout << "endpt 1: " << k->endpt1.x << ", " << k->endpt1.y << endl;
					cout << "endpt 2: " << k->endpt2.x << ", " << k->endpt2.y << endl;*/

					//in segement is horizontal line, endpt1 < endpt2 if endpt1.x < endpt2.x (y's will be equal)
					if(k->endpt1.x < k->endpt2.x)
					{
						//and check that the intersection point is between the endpoints
						if((x_int >= k->endpt1.x) && (x_int <= k->endpt2.x) && (y_int <= k->endpt1.y) && (y_int >= k->endpt2.y))
						{
							//make a new point from the intersection between the segment and sweep line
							//create a new pair from the two
							//and add it to the vector of intersected points for the sweep line
							Point intersect_point(x_int, y_int, k->endpt1.z);
							//Anchoring_Segment new_seg(*k);

							point_seg_pair intersect_pair = make_pair(intersect_point, *k);//don't need to make a new segment
							sweep_line.intersected_points.push_back(intersect_pair); //add the pair to the vector of pairs
						}
					}
					else
					{
						//if it is, we swap endpoints 1 and 2...
						//since I don't want to write swap for the Point class
						//we'll do it using temps...
						Point temp_1(k->endpt2.x, k->endpt2.y, k->endpt2.z);
						Point temp_2(k->endpt1.x, k->endpt1.y, k->endpt1.z);

						//and check that the intersection point is between the endpoints
						if((x_int >= temp_1.x) && (x_int <= temp_2.x) && (y_int <= temp_1.y) && (y_int >= temp_2.y))
						{
							//make a new point from the intersection between the segment and sweep line
							//create a new pair from the two
							//and add it to the vector of intersected points for the sweep line
							Point intersect_point(x_int, y_int, k->endpt1.z);
							//Anchoring_Segment new_seg(*k);

							point_seg_pair intersect_pair = make_pair(intersect_point, *k);//don't need to make a new segment
							sweep_line.intersected_points.push_back(intersect_pair); //add the pair to the vector of pairs
						}
					}
				}
				//check if segment slope is infinity, if it is, then sweep_slope is 0
				else if(segment_slope == std::numeric_limits<double>::infinity()) //check for vertical line
				{
					x_int = segment_x;
					y_int = point_y;
					
					/*//for debug
					cout << "Intersection at: " << x_int << ", " << y_int << endl;
					cout << "Segment info:";
					k->print_coords(cout);*/

					//(x_int, y_int) is the intersection between the sweep slope at that point and the anchoring segments
					//check that it is within the endpoints of the anchoring segment
					//first need to check if endpt1 < endpt2
					/*//for debug:
					cout << "CHECKING ENDPOINTS: " << endl;
					cout << "endpt 1: " << k->endpt1.x << ", " << k->endpt1.y << endl;
					cout << "endpt 2: " << k->endpt2.x << ", " << k->endpt2.y << endl;*/

					//if segment is vertical line, endpt1 < endpt2 if endpt1.y < endpt2.y (x's will be equal)
					if(k->endpt1.y < k->endpt2.y)
					{
						//and check that the intersection point is between the endpoints
						//if((x_int >= k->endpt1.x) && (x_int <= k->endpt2.x) && (y_int <= k->endpt1.y) && (y_int >= k->endpt2.y))
						if(k->endpt1.y <= y_int && y_int <= k->endpt2.y)
						{
							//make a new point from the intersection between the segment and sweep line
							//create a new pair from the two
							//and add it to the vector of intersected points for the sweep line
							Point intersect_point(x_int, y_int, k->endpt1.z);

							point_seg_pair intersect_pair = make_pair(intersect_point, *k);//don't need to make a new segment
							sweep_line.intersected_points.push_back(intersect_pair); //add the pair to the vector of pairs
						}
					}
					else
					{
						//if it is, we swap endpoints 1 and 2...
						//since I don't want to write swap for the Point class
						//we'll do it using temps...
						Point temp_1(k->endpt2.x, k->endpt2.y, k->endpt2.z);
						Point temp_2(k->endpt1.x, k->endpt1.y, k->endpt1.z);

						//and check that the intersection point is between the endpoints
						//if((x_int >= temp_1.x) && (x_int <= temp_2.x) && (y_int <= temp_1.y) && (y_int >= temp_2.y))
						if(y_int >= temp_1.y && y_int <= temp_2.y)
						{
							//make a new point from the intersection between the segment and sweep line
							//create a new pair from the two
							//and add it to the vector of intersected points for the sweep line
							Point intersect_point(x_int, y_int, k->endpt1.z);

							point_seg_pair intersect_pair = make_pair(intersect_point, *k);//don't need to make a new segment
							sweep_line.intersected_points.push_back(intersect_pair); //add the pair to the vector of pairs
						}
					}
				}
				else if(segment_slope > 0.0) //check for positive segment slope, negative sweep slope
				{
					x_int = calculate_x_intersection(y_intercept, segment_y_intercept, sweep_slope, segment_slope);
					y_int = calculate_y_intersection(y_intercept, segment_y_intercept, sweep_slope, segment_slope);
					
					//for debug
					cout << "Intersection at: " << x_int << ", " << y_int << endl;

					//(x_int, y_int) is the intersection between the sweep slope at that point and the anchoring segments
					//check that it is within the endpoints of the anchoring segment
					//first need to check if endpt1 < endpt2
					//for debug:
					cout << "CHECKING ENDPOINTS: " << endl;
					cout << "endpt 1: " << k->endpt1.x << ", " << k->endpt1.y << endl;
					cout << "endpt 2: " << k->endpt2.x << ", " << k->endpt2.y << endl;

					//in others, endpt1 < endpt2 if endpt1.x < endpt2.x (neither will be equal...)
					if(k->endpt1.x < k->endpt2.x)
					{
						//and check that the intersection point is between the endpoints
						if((x_int >= k->endpt1.x) && (x_int <= k->endpt2.x) && (y_int >= k->endpt1.y) && (y_int <= k->endpt2.y))
						{
							//make a new point from the intersection between the segment and sweep line
							//create a new pair from the two
							//and add it to the vector of intersected points for the sweep line
							Point intersect_point(x_int, y_int, k->endpt1.z);

							point_seg_pair intersect_pair = make_pair(intersect_point, *k);//don't need to make a new segment
							sweep_line.intersected_points.push_back(intersect_pair); //add the pair to the vector of pairs
							cout << "point pair added" << endl;
						}
					}
					else
					{
						//if it is, we swap endpoints 1 and 2...
						//since I don't want to write swap for the Point class
						//we'll do it using temps...
						Point temp_1(k->endpt2.x, k->endpt2.y, k->endpt2.z);
						Point temp_2(k->endpt1.x, k->endpt1.y, k->endpt1.z);

						//and check that the intersection point is between the endpoints
						if((x_int >= temp_1.x) && (x_int <= temp_2.x) && (y_int >= temp_1.y) && (y_int <= temp_2.y))
						{
							//make a new point from the intersection between the segment and sweep line
							//create a new pair from the two
							//and add it to the vector of intersected points for the sweep line
							Point intersect_point(x_int, y_int, k->endpt1.z);

							point_seg_pair intersect_pair = make_pair(intersect_point, *k);//don't need to make a new segment
							sweep_line.intersected_points.push_back(intersect_pair); //add the pair to the vector of pairs
							cout << "point pair added" << endl;
						}
					}
				}
				//other cases
				else
				{
					cout << "YO" << endl;
					x_int = calculate_x_intersection(y_intercept, segment_y_intercept, sweep_slope, segment_slope);
					y_int = calculate_y_intersection(y_intercept, segment_y_intercept, sweep_slope, segment_slope);
					
					//for debug
					//cout << "Intersection at: " << x_int << ", " << y_int << endl;

					//(x_int, y_int) is the intersection between the sweep slope at that point and the anchoring segments
					//check that it is within the endpoints of the anchoring segment
					//first need to check if endpt1 < endpt2
					/*//for debug:
					cout << "CHECKING ENDPOINTS: " << endl;
					cout << "endpt 1: " << k->endpt1.x << ", " << k->endpt1.y << endl;
					cout << "endpt 2: " << k->endpt2.x << ", " << k->endpt2.y << endl;*/

					//in others, endpt1 < endpt2 if endpt1.x < endpt2.x (neither will be equal...)
					if(k->endpt1.x < k->endpt2.x)
					{
						//cout << "endpt1 < endpt2" << endl;
						//and check that the intersection point is between the endpoints
						if((x_int >= k->endpt1.x) && (x_int <= k->endpt2.x) && (y_int <= k->endpt1.y) && (y_int >= k->endpt2.y))
						{
							//make a new point from the intersection between the segment and sweep line
							//create a new pair from the two
							//and add it to the vector of intersected points for the sweep line
							Point intersect_point(x_int, y_int, k->endpt1.z);

							point_seg_pair intersect_pair = make_pair(intersect_point, *k);//don't need to make a new segment
							sweep_line.intersected_points.push_back(intersect_pair); //add the pair to the vector of pairs
							//cout << "point pair added" << endl;
						}
					}
					else
					{
						//cout << "endpt1 > endpt2" << endl;
						//if it is, we swap endpoints 1 and 2...
						//since I don't want to write swap for the Point class
						//we'll do it using temps...
						Point temp_1(k->endpt2.x, k->endpt2.y, k->endpt2.z);
						Point temp_2(k->endpt1.x, k->endpt1.y, k->endpt1.z);

						//and check that the intersection point is between the endpoints
						if((x_int >= temp_1.x) && (x_int <= temp_2.x) && (y_int <= temp_1.y) && (y_int >= temp_2.y))
						{
							//make a new point from the intersection between the segment and sweep line
							//create a new pair from the two
							//and add it to the vector of intersected points for the sweep line
							Point intersect_point(x_int, y_int, k->endpt1.z);

							point_seg_pair intersect_pair = make_pair(intersect_point, *k);//don't need to make a new segment
							sweep_line.intersected_points.push_back(intersect_pair); //add the pair to the vector of pairs
							//cout << "point pair added" << endl;
						}
					}
				}
			}
		}
		cout << "number of intersected points on sweep line: " << sweep_line.intersected_points.size() << endl;
		sweep_line_vec.push_back(sweep_line);
	}
	/*//for debug
	cout << "sweep_line_vec.size: " << sweep_line_vec.size() << endl;
	for(auto i = sweep_line_vec.begin(); i != sweep_line_vec.end(); i++)
	{
		i->print_sweep_line_members(cout);
	}*/
}

//sets of segements crossing sweep plane with anchoring segments(?)
void union_sets(set<Anchoring_Segment> & original_set, set<Anchoring_Segment> & new_set)
{
	//go through all segments in new set,
	//look for them in original set,
	//if not in original set, insert into original set
	for(auto i = new_set.begin(); i != new_set.end(); i++)
	{
		std::set<Anchoring_Segment>::iterator it;
		it = original_set.find(*i);
		if(it == original_set.end())
		{
			original_set.insert(*i);
		}
	}
}

void difference_sets(set<Anchoring_Segment> & original_set, set<Anchoring_Segment> & remove_set)
{
	//go through all segments in remove_set
	//look for them in original_set
	//if they're in original_set, remove them
	for(auto i = remove_set.begin(); i != remove_set.end(); i++)
	{
		std::set<Anchoring_Segment>::iterator it;
		it = original_set.find(*i);
		if(it != original_set.end())
		{
			original_set.erase(it);
		}
	}
}

void difference_pt_sets(set<Point> & original_set, set<Point> & remove_set)
{
	//go through all segments in remove_set
	//look for them in original_set
	//if they're in original_set, remove them
	for(auto i = remove_set.begin(); i != remove_set.end(); i++)
	{
		std::set<Point>::iterator it;
		it = original_set.find(*i);
		if(it != original_set.end())
		{
			original_set.erase(it);
		}
	}
}

Point find_closest(Point bridge_p1, Point bridge_p2, Point p3)
{
	//if bridge_slope == 0
	if(bridge_p1.y == bridge_p2.y)
	{
		Point new_pt(p3.x, bridge_p1.y, bridge_p1.z);
		return new_pt;
	}
	//if bridge_slope == infinity
	else if(bridge_p1.x == bridge_p2.x)
	{
		Point new_pt(bridge_p1.x, p3.y, bridge_p1.z);
		return new_pt;
	}
	else
	{
		double bridge_slope = (bridge_p1.y - bridge_p2.y) / (bridge_p1.x - bridge_p2.x);
		double perp_slope = -1/bridge_slope;

		//equation of bridge_line is:
		//y-bridge_p1.y = bridge_slope * (x - bridge_p1.x)
		//equation of line perpendicular to bridge is:
		//y-p3.y = perp_slope * (x - p3.x)

		double x_int = ( p3.y - bridge_p1.y + (bridge_slope * bridge_p1.x) - (perp_slope * p3.x) )/ (bridge_slope - perp_slope);
		double y_int = perp_slope * (x_int - p3.x) + p3.y;
		double y_int2 = bridge_slope * (x_int - bridge_p1.x) + bridge_p1.y;
		//cout << "x_int: " << x_int << endl;
		//cout << "y_int: " << y_int << endl;
		//cout << "y_int2: " << y_int2 << endl;
		Point new_pt(x_int, y_int, bridge_p1.z);
		return new_pt;

	}
}

void remove_supported_pt_from_active_set(Point supported_pt, set<Point> & active_pts)
{
	//cout << "********inner loop" << endl;
	for(auto i = active_pts.begin(); i != active_pts.end(); i++)
	{
		//cout << "\tchecking ACTIVE point: ";
		//i->print_coords_with_z(cout);
		if(i->x == supported_pt.x && i->y == supported_pt.y && i->z == supported_pt.z)
		{
			//cout << "\tpoint found!" << endl;
			active_pts.erase(i);
			//cout << "\tpoint erased." << endl;
			break;
		}
	}
}

//TODO: snap should just take the bridge, because each bridge has a set of points supported by bridge called supported_points
//maybe also need to send the active events, too.  Does that change with each sweep direction? Hmmm...
void snap(Bridge & best_bridge, set<Point> & active_pts)
{
	//cout << "*******************SNAPPING..." << endl;
	//for debug:
	cout << "*******Active points size before: " << active_pts.size() << endl;
	/*for(auto i = active_pts.begin(); i != active_pts.end(); i++)
	{
		i->print_coords_with_z(cout);
	}*/

	//for debug:
	cout << "Bridge data:" << endl;
	best_bridge.print_bridge_members(cout);
	cout << "Supported points: " << endl;
	for(auto j = best_bridge.supported_points.begin(); j != best_bridge.supported_points.end(); j++)
	{
		j->print_coords_with_z(cout);
	}
	
	//cout << "removing supported points from set..." << endl;
	//remove supported points from set of points that need support (active_pts)
	for(auto i = best_bridge.supported_points.begin(); i != best_bridge.supported_points.end(); i++)
	{
		cout << "START OUTER LOOP" << endl;
		cout << "checking supported point:";
		i->print_coords_with_z(cout);
		remove_supported_pt_from_active_set(*i, active_pts);
	}
	//cout << "DONE WITH POINTS" << endl;
	
	//for debug:
	cout << "*******Active points size after erasure: " << active_pts.size() << endl;
	/*for(auto i = active_pts.begin(); i != active_pts.end(); i++)
	{
		i->print_coords_with_z(cout);
	}*/

	//bridge comments for scad outfile
	out_file << "//STARTING NEW BRIDGE DATA" << endl;
	out_file << "//bridge data:" << endl;
	out_file << "/*" <<endl;
	best_bridge.print_bridge_members(out_file);
	out_file << "//supporting points:" << endl;
	for(auto i = best_bridge.supported_points.begin(); i != best_bridge.supported_points.end(); i++)
	{
		i->print_coords_with_z(out_file);
	}
	out_file << "*/" <<endl;

	//cout << "dropping slanted pillars" << endl;
	//drop slanted pillar for each supported point
	for(auto i = best_bridge.supported_points.begin(); i != best_bridge.supported_points.end(); i++)
	{
		//if(i->z != best_bridge.p1.z) //if the supported point is *above* the bridge (not on the same level as bridge)
		//{
			//pillar(x, y, height of pillar base, vertical height of pillar)
			//slanted_pillar(x1, y1, z1, x2, y2, z2) //where x1, y1, z1 are coords of pt that needs support
			//and x2, y2, z2 are coords of closest point on bridge to pt that needs support
			//NOTE: Also need to work epsilon in here...?????
			Point point_on_bridge = find_closest(best_bridge.p1, best_bridge.p2, *i);
			out_file << "//slanted pillar" << endl;
			out_file << "\tslanted_pillar(" << i->x << ", " << i->y << ", " << i->z << ", " <<
				point_on_bridge.x << ", " << point_on_bridge.y << ", " <<  best_bridge.height << ");" << endl;
		//}
	}

	/*//for debug:
	cout << "*********Active points size after dropped pillars: " << active_pts.size() << endl;
	for(auto i = active_pts.begin(); i != active_pts.end(); i++)
	{
		i->print_coords_with_z(cout);
	}*/
	
	//lay bar from bridge endpt1 to endpt2
	//and add pts to active_pts
	//first check for horizontal
	if(best_bridge.p1.y == best_bridge.p2.y)//if horizontal, use bridge1
	{
		out_file << "//bridge:" << endl;
		out_file << "\tbridge1(" << best_bridge.p1.x << ", " << best_bridge.p1.y << ", " << best_bridge.p2.x << ", " << best_bridge.p2.y << ", " << best_bridge.height << ");" << endl;
	}
	else //otherwise use bridge
	{
		out_file << "//bridge:" << endl;
		out_file << "\tbridge(" << best_bridge.p1.x << ", " << best_bridge.p1.y << ", " << best_bridge.p2.x << ", " << best_bridge.p2.y << ", " << best_bridge.height << ");" << endl;
	}
	
	//cout << "Bridge endpt 1: ";
	//best_bridge.p1.print_coords_with_z(cout);
	if(best_bridge.p1.x != 0 && best_bridge.p1.y != 0 && best_bridge.p1.z != 0)
	{
			active_pts.insert(best_bridge.p1);
	}
	
	//cout << "Bridge endpt 2: ";
	//best_bridge.p2.print_coords_with_z(cout);
	if(best_bridge.p2.x != 0 && best_bridge.p2.y != 0 && best_bridge.p2.z != 0)
	{
			active_pts.insert(best_bridge.p2);
	}

	//for debug
	cout << "Active points size after adding endpts: " << active_pts.size() << endl;
	/*for(auto i = active_pts.begin(); i != active_pts.end(); i++)
	{
		i->print_coords_with_z(cout);
	}*/
}

void send_remaining_points_to_scad(set<Point> &active_pts)
{
	out_file << "//remaining active points, dropping pillars to surface" << endl;
	for(auto i = active_pts.begin(); i != active_pts.end(); i++)
	{
		out_file << "\tpillar(" << i->x << ", " << i->y << ", 0, " << i->z << ");" << endl;
		if(i->z > 3.0)
		{
			out_file << "\tfoot(" << i->x << ", " << i->y << ");" << endl;
		}
		out_file "\traft(" << i->x << ", " << i->y << ");" << endl;
	}
	//and output final closing brace for the union of all supports
	out_file << "}" << endl;
}

void setup_scad()
{
	out_file << "//scad pillars and bars generated by code" << endl << endl;
	out_file << "$fa = 5;" << endl;
	out_file << "$fn = 5;" << endl;
	out_file << "radius = 1.0;" << endl;
	out_file << "inner = 0.5;" << endl;
	out_file << "delta = 0.5;" << endl;
	out_file << "height = 0.4;" << endl;
	out_file << "foot_radius = 2.5;" << endl;
	out_file << "foot_height = 2.5;" << endl;
	out_file << "raft_width = 7;" << endl;
	out_file << "raft_height = 0.5;" << endl;
	out_file << "epsilon = 0.01;" << endl << endl;
	out_file << "module raft(x_coord, y_coord)" << endl;
	out_file << "{" << endl;
	out_file << "\ttranslate([x_coord, y_coord, 0])" << endl
	out_file << "\tcube([raft_width, raft_width, raft_height], center = true);" << endl;
	out_file << "}" << endl;
	out_file << "module foot(x_coord, y_coord)" << endl;
	out_file << "{" << endl;
	out_file << "\ttranslate([x_coord, y_coord, 0])" << endl;
	out_file << "\tdifference()" << endl;
	out_file << "\t{" << endl;
	out_file << "\t\tcylinder(foot_height, foot_radius, radius);" << endl;
	out_file << "\t\ttranslate([0, 0, -epsilon])" << endl;
	out_file << "\t\tcylinder(foot_height+2*epsilon, foot_radius-inner, radius-inner);" << endl;
	out_file << "\t}" << endl;
	out_file << "}" << endl;
	out_file << "module circle1(x_coord, y_coord, z_coord)" << endl;
	out_file << "{" << endl;
    out_file << "\ttranslate([x_coord, y_coord, z_coord])" << endl;
    out_file << "\tcylinder(.2, radius-.5, radius-.5);" << endl;
	out_file << "}" << endl;
	out_file << "module circle2(x_coord, y_coord, z_coord)" << endl;
	out_file << "{" << endl;
    out_file << "\ttranslate([x_coord, y_coord, z_coord])" << endl;
    out_file << "\tcylinder(.2, radius, radius);" << endl;
	out_file << "}" << endl;
	out_file << "module slanted_pillar(x_coord1, y_coord1, z_coord1, x_coord2, y_coord2, z_coord2)" << endl;
	out_file << "{" << endl;
    out_file << "\thull()" << endl;
    out_file << "\t{" << endl;
    out_file << "\t\tcircle1(x_coord1, y_coord1, z_coord1);" << endl;
    out_file << "\t\tcircle2(x_coord2, y_coord2, z_coord2);" << endl;
    out_file << "\t}" << endl;
	out_file << "}" << endl;
	out_file << "module pillar(x_coord, y_coord, z_coord, z_height)" << endl;
	out_file << "{" << endl;
	out_file << "\tif(z_height > 5)" << endl;
	out_file << "\t{" << endl;
	out_file << "\t\thull()" << endl;
	out_file << "\t\t{" << endl;
	out_file << "\t\t\tcircle1(x_coord, y_coord, z_height);" << endl;
	out_file << "\t\t\tcircle2(x_coord, y_coord, z_height-5);" << endl;
	out_file << "\t\t}" << endl;
	out_file << "\t\ttranslate([x_coord, y_coord, z_coord])" << endl;
	out_file << "\t\t{" << endl;
	out_file << "\t\t\tdifference()" << endl;
	out_file << "\t\t\t{" << endl;
	out_file << "\t\t\t\tlinear_extrude(z_height-5+epsilon)" << endl;
	out_file << "\t\t\t\t{" << endl;
	out_file << "\t\t\t\t\tcircle(radius);" << endl;
	out_file << "\t\t\t\t}" << endl;
	out_file << "\t\t\t\ttranslate([0, 0, -epsilon])" << endl;
	out_file << "\t\t\t\tlinear_extrude(z_height-5)" << endl;
	out_file << "\t\t\t\t{" << endl;
	out_file << "\t\t\t\t\tcircle(radius-inner);" << endl;
	out_file << "\t\t\t\t}" << endl;
	out_file << "\t\t\t}" << endl;
	out_file << "\t\t}" << endl;
	out_file << "\t}" << endl;
	out_file << "\telse" << endl;
	out_file << "\t{" << endl;
	out_file << "\t\ttranslate([x_coord, y_coord, z_coord])" << endl;
	out_file << "\t\t{" << endl;
	out_file << "\t\t\tdifference()" << endl;
	out_file << "\t\t\t{" << endl;
	out_file << "\t\t\t\tlinear_extrude(z_height)" << endl;
	out_file << "\t\t\t\t{" << endl;
	out_file << "\t\t\t\t\tcircle(radius);" << endl;
	out_file << "\t\t\t\t}" << endl;
	out_file << "\t\t\t\ttranslate([0, 0, -epsilon])" << endl;
	out_file << "\t\t\t\tlinear_extrude(z_height+2*epsilon)" << endl;
	out_file << "\t\t\t\t{" << endl;
	out_file << "\t\t\t\t\tcircle(radius-inner);" << endl;
	out_file << "\t\t\t\t}" << endl;
	out_file << "\t\t\t}" << endl;
	out_file << "\t\t}" << endl;
	out_file << "\t}" << endl;
	out_file << "}" << endl;
	out_file << "//does not work for horizontal bridges, use bridge1" << endl;
	out_file << "module bridge(x0, y0, x1, y1, z_height)" << endl;
	out_file << "{" << endl;
    out_file << "translate([0, 0, z_height])" << endl;
    out_file << "linear_extrude(height)" << endl;
    out_file << "polygon(points = [[x0+delta, y0], [x0-delta, y0], [x1-delta, y1], [x1+delta, y1]]);" << endl;
	out_file << "}" << endl << endl;
	out_file << "//does not work for vertical bridges, use bridge" << endl;
	out_file << "module bridge1(x0, y0, x1, y1, z_height)" << endl;
	out_file << "{" << endl;
    out_file << "translate([0, 0, z_height])" << endl;
    out_file << "linear_extrude(height)" << endl;
    out_file << "polygon(points = [[x0, y0+delta], [x0, y0-delta], [x1, y1-delta], [x1, y1+delta]]);" << endl;
	out_file << "}" << endl << endl;
	out_file << "union()" << endl;
	out_file << "{" << endl; //union opening brace
}

void output_pts_to_support_to_scad(set<Point> &active_pts, string color)
{
	for(auto i = active_pts.begin(); i != active_pts.end(); i++)
	{
		out_file << "translate(["<< i->x << ", " <<  i->y << ", " << i->z << "])" << endl;
		out_file << "{" << endl;
    	out_file << "\tcolor(\"" << color << "\", 0.5)" << endl;
    	out_file << "\tsphere(0.75, center = true);" << endl;
		out_file << "}" << endl;
	}
}

//input: set of points that need support from getPoints
//output: bridge structure
void generate_scaffolding(set<Point> points_needing_support)
{

}

