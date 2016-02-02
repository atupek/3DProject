//generate_scaffolding.cpp
//this is algorithm 2 from Bridging the Gap
#include "generate_scaffolding.h"
#include "select_bridge.h"
#include "bridge.h"
#include <limits> //for infinity
//#include "pillar.h"
//#include "cube_primitive.h"

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
	/*cout << "TRUE SEGMENT: " << endl;
	cout << "true, new_x1: " << new_segment.new_x1 << endl;
	cout << "true, new_y1: " << new_segment.new_y1 << endl;
	new_segment.print_coords(cout);*/

	Anchoring_Segment new_segment1(pt, slope, false);
	anchor_segments.insert(new_segment1);
	/*cout << "FALSE SEGMENT: " << endl;
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
			Event changed_event(*set_it); //added this line
			
			//and then erase the old one
			events.erase(*set_it);//added this line
			
			//and add the segment to the new one
			changed_event.event_segments.push_back(*i);//added this line
			
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
//TODO: ISSUES WITH SLOPE == 0 and SLOPE == INF
//NOT TAKEN CARE OF BY if(slope1 == slope2)
//NEED TO FIX IN BOTH X and Y intersection
//THINK: DO I NEED THE CONDITIONAL ELSEWHERE, LIKE BEFORE I CALL THE FUNCTION?
//if slope = 0, x-intersection DNE unless y = 0;  y = b
//if slope = inf, x-intersection = x; x = b
//DONE>>>>>>think I fixed it!
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

//instead of putting points into set of points, need to put them into appropriate anchoring segment's intersected points vector
void find_intersections(set<Event> & events, vector<double> sweep_directions, int sweep_index, set<Anchoring_Segment> &intersect_segments)
{
	cout << "FINDING INTERSECTIONS" << endl;
	double sweep_slope = sweep_directions[sweep_index];
	//find y-intercept of line at point of event
	//then find y-intercept of anchoring segment line
	//then find intersection of both lines
	//if that intersection point is between the anchoring segment's endpoints
	//then add that intersection point to the anchoring segment's vector of intersection points
	for(auto i = events.begin(); i != events.end(); i++)
	{
		//for debug
		cout << "****************EVENT INFO:*********************" << endl;
		i->print_event_members(cout);

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
			for(auto k = j->event_segments.begin(); k != j->event_segments.end(); k++)
			{
				//line 1
				double segment_x = k->endpt1.x;
				double segment_y = k->endpt1.y;
				double segment_slope = k->slope;
				double segment_y_intercept = segment_y - segment_slope * segment_x;
				//double x_int = calculate_x_intersection(y_intercept, segment_y_intercept, sweep_slope, segment_slope);
				//double y_int = calculate_y_intersection(y_intercept, segment_y_intercept, sweep_slope, segment_slope);
				double x_int;
				double y_int;
				if(segment_slope == 0)
				{
					x_int = point_x;
					y_int = segment_y;
				}
				else if(segment_slope == std::numeric_limits<double>::infinity())
				{
					x_int = segment_x;
					y_int = point_y;
				}
				else
				{
					x_int = calculate_x_intersection(y_intercept, segment_y_intercept, sweep_slope, segment_slope);
					y_int = calculate_y_intersection(y_intercept, segment_y_intercept, sweep_slope, segment_slope);
				}

				//for debug //OKAY THROUGH THIS POINT...
				//cout << "Intersection at: " << x_int << ", " << y_int << endl;

				//(x_int, y_int) is the intersection between the sweep slope at that point and the anchoring segments
				//check that it is within the endpoints of the anchoring segment
				//first need to check if endpt1 < endpt2
				//for debug:
				/*cout << "CHECKING ENDPOINTS: " << endl;
				cout << "endpt 1: " << k->endpt1.x << ", " << k->endpt1.y << endl;
				cout << "endpt 2: " << k->endpt2.x << ", " << k->endpt2.y << endl;*/
				
				if(k->endpt1.x < k->endpt2.x)
				{
					cout << endl << "TRUE, IN LOOP" << endl;
					//and check that the intersection point is between the endpoints
					if((x_int >= k->endpt1.x) && (x_int <= k->endpt2.x) && (y_int <= k->endpt1.y) && (y_int >= k->endpt2.y))
					{
						Point new_point(x_int, y_int, k->endpt1.z);
						Anchoring_Segment new_seg(*k);

						//check if anchoring segment is already in the set
						auto set_it = intersect_segments.find(new_seg);
						if(set_it != intersect_segments.end()) // if it's already in intersect_segments set
						{	
							//can't change items in a set, so have to create a new one
							Anchoring_Segment changed_segment(*set_it);
							
							//and then erase the old one
							intersect_segments.erase(*set_it);//added this line
							
							//and add the point to the new one's point vector if it's not already there
							int point_already_in_vec = 0;
							for(auto m = changed_segment.intersected_points.begin(); m != changed_segment.intersected_points.end(); m++)
							{
								if(*m == new_point)
								{
									point_already_in_vec++;
								}
							}
							//if the point is not found, then add it in
							if(point_already_in_vec == 0)
							{
								changed_segment.intersected_points.push_back(new_point);
								//FOR DEBUG
								/*cout << "Added point to segment's intersected points: ";
								new_point.print_coords(cout);*/
							}
							
							//and then insert the new segment into the set of segments
							intersect_segments.insert(changed_segment);
							//FOR DEBUG
							/*cout << "Added changed segment: " << endl;
							cout << "\t";
							changed_segment.print_coords(cout);
							cout << "Segment intersected points: " << endl;
							changed_segment.print_intersect_pts(cout);
							cout << endl;*/
						}
						else // if it's not already in segment set, then add it
						{
							//and add the point to the new one's point vector if it's not already there
							int point_already_in_vec = 0;
							for(auto m = new_seg.intersected_points.begin(); m != new_seg.intersected_points.end(); m++)
							{
								if(*m == new_point)
								{
									point_already_in_vec++;
								}
							}
							//if the point is not found, then add it in
							if(point_already_in_vec == 0)
							{
								new_seg.intersected_points.push_back(new_point);
								//FOR DEBUG
								/*cout << "Added point to segment's intersected points: ";
								new_point.print_coords(cout);*/
							}
							//new_seg.intersected_points.push_back(new_point); //added this line...but moved up to check if already there
							intersect_segments.insert(new_seg);
							//FOR DEBUG
							/*cout << "Added new segment: " << endl;
							cout << "\t";
							new_seg.print_coords(cout);
							cout << "Segment intersected points: " << endl;
							new_seg.print_intersect_pts(cout);
							cout << endl;*/
						}
					}
				}
				//this is in case endpt1 > endpt2
				else
				{
					cout << endl << "FALSE, IN SECOND LOOP" << endl;
					//if it is, we swap endpoints 1 and 2...
					//since I don't want to write swap for the Point class
					//we'll do it using temps...
					Point temp_1(k->endpt2.x, k->endpt2.y, k->endpt2.z);
					Point temp_2(k->endpt1.x, k->endpt1.y, k->endpt1.z);

					//and check that the intersection point is between the endpoints
					if((x_int >= temp_1.x) && (x_int <= temp_2.x) && (y_int <= temp_1.y) && (y_int >= temp_2.y))
					{
						Point new_point(x_int, y_int, k->endpt1.z);
						Anchoring_Segment new_seg(*k);

						//check if anchoring segment is already in the set
						auto set_it = intersect_segments.find(new_seg);
						if(set_it != intersect_segments.end()) // if it's already in intersect_segments set
						{	
							//can't change items in a set, so have to create a new one
							Anchoring_Segment changed_segment(*set_it);
							
							//and then erase the old one
							intersect_segments.erase(*set_it);
							
							//and add the point to the new one's point vector if it's not already there
							int point_already_in_vec = 0;
							for(auto m = changed_segment.intersected_points.begin(); m != changed_segment.intersected_points.end(); m++)
							{
								if(*m == new_point)
								{
									point_already_in_vec++;
								}
							}
							//if the point is not found, then add it in
							if(point_already_in_vec == 0)
							{
								changed_segment.intersected_points.push_back(new_point);
								//FOR DEBUG
								/*cout << "Added point to segment's intersected points: ";
								new_point.print_coords(cout);*/
							}
							
							//and then insert the new segment into the set of segments
							intersect_segments.insert(changed_segment);
							//FOR DEBUG
							/*cout << "Added changed segment: " << endl;
							cout << "\t";
							changed_segment.print_coords(cout);
							cout << "Segment intersected points: " << endl;
							changed_segment.print_intersect_pts(cout);
							cout << endl;*/
						}
						else // if it's not already in segment set, then add it in
						{
							int point_already_in_vec = 0;
							for(auto m = new_seg.intersected_points.begin(); m != new_seg.intersected_points.end(); m++)
							{
								if(*m == new_point)
								{
									point_already_in_vec++;
								}
							}
							//if the point is not found, then add it in
							if(point_already_in_vec == 0)
							{
								new_seg.intersected_points.push_back(new_point);
								//FOR DEBUG
								/*cout << "Added point to segment's intersected points: ";
								new_point.print_coords(cout);*/
							}
							//new_seg.intersected_points.push_back(new_point); //added this line...but moved up to check if already there
							intersect_segments.insert(new_seg);
							//FOR DEBUG
							/*cout << "Added new segment: " << endl;
							cout << "\t";
							new_seg.print_coords(cout);
							cout << "Segment intersected points: " << endl;
							new_seg.print_intersect_pts(cout);
							cout << endl;*/
						}
					}
				}
			}
		}
	}
	/*
	//for debug:
	cout << "Intersect Segments size: " << intersect_segments.size() << endl;
	int j=0;
	for(auto i = intersect_segments.begin(); i != intersect_segments.end(); i++)
	{
		cout << "Number of intersect points for segment " << j << ": " << i->intersected_points.size() << endl;
		j++;
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

//TODO: snap should just take the bridge, because each bridge has a set of points supported by bridge called supported_points
//maybe also need to send the active events, too.  Does that change with each sweep direction? Hmmm...
//void snap(Bridge & best_bridge, set<Point> & points_supported_by_bridge)
//void snap(Bridge &best_bridge, set<Point> &points_supported_by_bridge, vector<Pillar> &pillars, vector<Cube_Primitive> &cubes, double dist_to_obj_above)
void snap(Bridge & best_bridge, set<Event> & _active_events)
{
	//STARTING WORK HERE:
	//we must remove the supported points from the set of active events.
	//this code below is not going to work because can't compare a point to an event...damn.
	//so I'll need to take the point, make an event out of it, and then do the removal,
	//but that means that I need to rewrite the < comparitor for the event class.  Damn.
	/*for(auto i = 0; i < best_bridge.supported_points.size(); i++)
	{
		std::set<Event>::iterator it;
		it = _active_events.find(*i);
		if(it != _active_events.end())
		{
			_active_events.erase(it);
		}
	}*/

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

