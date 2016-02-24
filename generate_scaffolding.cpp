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
		//cout << "**************** I EVENT INFO:*********************" << endl;
		//i->print_event_members(cout);

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
				//other cases
				else
				{
					x_int = calculate_x_intersection(y_intercept, segment_y_intercept, sweep_slope, segment_slope);
					y_int = calculate_y_intersection(y_intercept, segment_y_intercept, sweep_slope, segment_slope);
					
					/*//for debug
					cout << "Intersection at: " << x_int << ", " << y_int << endl;*/

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
					//and check that the intersection point is between the endpoints
						if((x_int >= k->endpt1.x) && (x_int <= k->endpt2.x) && (y_int <= k->endpt1.y) && (y_int >= k->endpt2.y))
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
						if((x_int >= temp_1.x) && (x_int <= temp_2.x) && (y_int <= temp_1.y) && (y_int >= temp_2.y))
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
			}
		}
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
/*
Pillar make_pillar(Point point1, double height)
{
	cout << "Making pillar..." << endl;
	Pillar temp = Pillar(point1.x, point1.y, point1.z, point1.z+height); 
	return temp;
}

Cube_Primitive make_cube_primitive(Point point1, Point point2)
{
	cout << "making cube..." << endl;
	Cube_Primitive temp = Cube_Primitive(point1.x, point1.y, point2.x, point2.y, point1.z, point1.z+0.4);
	return temp;
}*/

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

//TODO: snap should just take the bridge, because each bridge has a set of points supported by bridge called supported_points
//maybe also need to send the active events, too.  Does that change with each sweep direction? Hmmm...
//void snap(Bridge & best_bridge, set<Point> & points_supported_by_bridge)
//void snap(Bridge &best_bridge, set<Point> &points_supported_by_bridge, vector<Pillar> &pillars, vector<Cube_Primitive> &cubes, double dist_to_obj_above)
void snap(Bridge & best_bridge, set<Point> & active_pts)
{
	//remove supported points from set of points that need support (active_pts)
	//damn it, supported_pts is a vector, not a set...so I don't need to use the difference_pt_set
	//I can just go thorough the set and erase it...
	//for debug:
	cout << "*******Active points size before: " << active_pts.size() << endl;
	for(auto i = active_pts.begin(); i != active_pts.end(); i++)
	{
		i->print_coords_with_z(cout);
	}
	for(auto i = best_bridge.supported_points.begin(); i != best_bridge.supported_points.end(); i++)
	{
		std::set<Point>::iterator it;
		it = active_pts.find(*i);
		if(it != active_pts.end())
		{
			active_pts.erase(it);
			//cout << "ERASED POINT" << endl;
		}
	}
	//for debug:
	cout << "*******Active points size after erasure: " << active_pts.size() << endl;
	for(auto i = active_pts.begin(); i != active_pts.end(); i++)
	{
		i->print_coords_with_z(cout);
	}

	//active_pts.insert(best_bridge.p1);
	//active_pts.insert(best_bridge.p2);

	//drop pillar for each supported point
	//add point at bottom of pillar to set of active points
	//filename << "\tpillar(" << i->p2.x << ", " << i->p2.y << ", " << i->height << ", " << (i->p2.z - i->height) << ");" << endl;
	for(auto i = best_bridge.supported_points.begin(); i != best_bridge.supported_points.end(); i++)
	{
		if(i->z != best_bridge.p1.z) //if the supported point is *above* the bridge (not on the same level as bridge)
		{
			//pillar(x, y, height of pillar base, vertical height of pillar)
			//NOTE: Also need to work epsilon in here...
			cout << "\tpillar(" << i->x << ", " << i->y << ", " << best_bridge.height << ", " << (i->z - best_bridge.height) << ");" << endl;
			//Point lowered_point(i->x, i->y, best_bridge.height);
			//active_pts.insert(lowered_point);
		}
		Point lowered_point(i->x, i->y, best_bridge.height);
		active_pts.insert(lowered_point);
	}
	//for debug:
	cout << "*********Active points size after dropped pillars: " << active_pts.size() << endl;
	for(auto i = active_pts.begin(); i != active_pts.end(); i++)
	{
		i->print_coords_with_z(cout);
	}

	//lay bar from dropped pillar to bridge for each supported point
	//as long as point is not actually on the bridge...TODO....THIS CONDITIONAL
	for(auto i = best_bridge.supported_points.begin(); i != best_bridge.supported_points.end(); i++)
	{
		//find closest point on bridge line to the dropped pillar
		//and call bridge on that (unless y1 == y2, and then call bridge1)
		Point closest_point = find_closest(best_bridge.p1, best_bridge.p2, *i);
		//cout << "Closest points: " << endl;
		//closest_point.print_coords_with_z(cout);
		if(best_bridge.p1.x == best_bridge.p2.x)//if original bridge is vertical, attaching will be horizontal use bridge1
		{
			//filename << "\tbridge1(" << i->p1.x << ", " << i->p1.y << ", " << i->p2.x << ", " << i->p2.y << ", " << i->height << ");" << endl;
			cout << "\tbridge1(" << i->x << ", " << i->y << ", " << closest_point.x << ", " << closest_point.y << ", " << best_bridge.height << ");" << endl;
		}
		else
		{
			//filename << "\tbridge(" << i->p1.x << ", " << i->p1.y << ", " << i->p2.x << ", " << i->p2.y << ", " << i->height << ");" << endl;
			cout << "\tbridge(" << i->x << ", " << i->y << ", " << closest_point.x << ", " << closest_point.y << ", " << best_bridge.height << ");" << endl;
		}

	}

	/*//for 'testing...' //seems to work...definitely not exhaustive
	Point test_0(0, 0, 0);
	Point test_1(10, 5, 0);
	Point test_2(5, 10, 0);
	Point test_3 = find_closest(test_0, test_1, test_2);*/

	//lay bar from bridge endpt1 to endpt2
	//and add pts to active_pts
	//first check for horizontal
	if(best_bridge.p1.y == best_bridge.p2.y)//if horizontal, use bridge1
	{
		cout << "//THE MAIN BRIDGE:" << endl;
		cout << "\tbridge1(" << best_bridge.p1.x << ", " << best_bridge.p1.y << ", " << best_bridge.p2.x << ", " << best_bridge.p2.y << ", " << best_bridge.height << ");" << endl;
	}
	else //otherwise use bridge
	{
		cout << "//THE MAIN BRIDGE:" << endl;
		cout << "\tbridge(" << best_bridge.p1.x << ", " << best_bridge.p1.y << ", " << best_bridge.p2.x << ", " << best_bridge.p2.y << ", " << best_bridge.height << ");" << endl;
	}
	
	/*cout << "Bridge endpt 1: ";
	best_bridge.p1.print_coords_with_z(cout);
	cout << "Bridge endpt 2: ";
	best_bridge.p2.print_coords_with_z(cout);*/

	/*
	Point new_test_pt(2, 2, 2);
	active_pts.insert(new_test_pt);
	Point new_test_pt1(18, 9, 17);
	active_pts.insert(new_test_pt1);*/
	
	cout << "Active points before adding endpts: " << active_pts.size() << endl;
	for(auto i = active_pts.begin(); i != active_pts.end(); i++)
	{
		i->print_coords_with_z(cout);
	}

	cout << "Bridge endpt 1: ";
	best_bridge.p1.print_coords_with_z(cout);
	active_pts.insert(best_bridge.p1);
	cout << "Bridge endpt 2: ";
	best_bridge.p2.print_coords_with_z(cout);
	active_pts.insert(best_bridge.p2);

	Point stupid_pt(19, 15, 17);
	active_pts.insert(stupid_pt);

	//WHERE I AM: NEED TO TEST THIS....
	//dang it...I think I need to add a z-comparison into the < operator for points?
	//but that's not why bridge endpt1 isn't being added to the set...
	cout << "Active points size after adding endpts: " << active_pts.size() << endl;
	for(auto i = active_pts.begin(); i != active_pts.end(); i++)
	{
		i->print_coords_with_z(cout);
	}


	//generate pillar from supported point (x, y, z1) to point on bridge (x, y, z2)
	//however we're going to do that...

	//higher bridge endpoint needs to be brought down to z-height of lower endpoint
	//and pillar producted to upper z-height
	/*if(best_bridge.p1.z > best_bridge.p2.z) //if z of p1 > z of p2, z of p1 = z of p2
	{
		best_bridge.p1.z = best_bridge.p2.z;
	}
	else
	{
		best_bridge.p2.z = best_bridge.p1.z; //otherwise z of p2 >= z of p1 and z p2 should be changed to reflect that
	}*/
	//and now produce pillar, however we're going to do that...

	//Okay, once that is done we need to add the endpoints of the bridge into the set of active events
	//so we need to create a new event
	/*Event new_event1(best_bridge.p1);
	Event new_event2(best_bridge.p2);
	_active_events.insert(new_event1);
	_active_events.insert(new_event2);*/

}

//input: set of points that need support from getPoints
//output: bridge structure
void generate_scaffolding(set<Point> points_needing_support)
{

}

