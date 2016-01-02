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
	//new_segment.print_coords(cout);

	Anchoring_Segment new_segment1(pt, slope, false);
	anchor_segments.insert(new_segment1);
	//new_segment1.print_coords(cout);

	//cout << "(in draw line)Anchoring segment size: " << anchor_segments.size() << endl;

	//for testing union_sets:
	//test_seg.insert(new_segment1);
}

void create_anchoring_segments(set<Point> &point_set, set<Bridge> &bridge_set, set<Anchoring_Segment> &anchor_segments, vector<double> &sweep_direction, int i)
{
	double plane = sweep_direction[i];
	//for each Point in point_set, create anchor segment of length max_length orthoganl to sweep_direction, centered on Point
	//for_each(point_set.begin(), point_set.end(), draw_line(sweep_direction[i]));
	for(auto i = point_set.begin(); i != point_set.end(); i++)
	{
		draw_line(*i, plane, anchor_segments);
		//new_create_events(*i, plane); //NO, NO, NO, NO, NO! get rid of new_create_events
	}
	for(auto i = bridge_set.begin(); i != bridge_set.end(); i++)
	{
		//TODO************************************************
		//should check if endpoit is open or closed before doing this, I think...
		if(i->pt1_open)
		{
			draw_line(i->p1, plane, anchor_segments);
		}
		if(i->pt2_open)
		{
			draw_line(i->p2, plane, anchor_segments);
		}
	}
}


//TODO: figure out why can't push_back to vector of segments when event already exists
void create_events(set<Anchoring_Segment> &_segments, set<Event> & events)
{
	//cout << "CREATE EVENTS CALLED" << endl;

	for(auto i = _segments.begin(); i != _segments.end(); i++)
	{
		Event new_event(i->endpt1);
		new_event.event_segments.push_back(*i);
		auto set_it = events.find(new_event);
		if(set_it != events.end()) // if it's already in event set, add segment to its vector of segments
		{
			//cout << "event found" << endl;
			//cout << "Event segment size: " << set_it->event_segments.size() << endl;
			//*set_it->event_segments.push_back(*i); //WHY THIS NO WORK?!?!?
		}
		else // if it's not already in event set, then add it
		{
			events.insert(new_event);
		}
	}
}

double calculate_x_intersection(double y_intercept1, double y_intercept2, double slope1, double slope2)
{
	double numerator = y_intercept2 - y_intercept1;
	double denominator = slope1 - slope2;
	return numerator/denominator;
}

double calculate_y_intersection(double y_intercept1, double y_intercept2, double slope1, double slope2)
{
	double numerator = (y_intercept1 * slope2) - (y_intercept2 * slope1);
	double denominator = slope2 - slope1;
	return numerator/denominator;

}

void find_intersections(set<Event> & events, vector<double> sweep_directions, int sweep_index, set<Point> &intersect_pts)
{
	double sweep_slope = sweep_directions[sweep_index];
	//find y-intercept of line at point of event
	//then find y-intercept of anchoring segment line
	//then find intersection of both lines
	//if that intersection point is between endpoints of the anchoring segments
	//then add that intersection point to the list of points sent to algorithm 3
	for(auto i = events.begin(); i != events.end(); i++)
	{
		//line 0
		double point_x = i->p1.x;
		double point_y = i->p1.y;
		double y_intercept = point_y - sweep_slope * point_x;
		//equation of line at the event point:
		// y = sweep_slope * x + y_intercept
		
		/*//for debug
		cout << "****************************EQUATION OF LINE: " << endl;
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
				double x_int = calculate_x_intersection(y_intercept, segment_y_intercept, sweep_slope, segment_slope);
				double y_int = calculate_y_intersection(y_intercept, segment_y_intercept, sweep_slope, segment_slope);

				//for debug
				//cout << "Intersection at: " << x_int << ", " << y_int << endl;

				//(x_int, y_int) is the intersection between the sweep slope at that point and the anchoring segments
				//check that it is within the endpoints of the anchoring segment
				//first need to check if endpt1 < endpt2
				/*//for debug:
				cout << "CHECKING ENDPOINTS: " << endl;
				cout << "endpt 1: " << k->endpt1.x << ", " << k->endpt1.y << endl;
				cout << "endpt 2: " << k->endpt2.x << ", " << k->endpt2.y << endl;*/
				
				if(k->endpt1.x < k->endpt2.x)
				{
					cout << "IN HERE...." << endl;
					/*//for debug
					cout << "x_int: " << x_int << "\tk->endpt1.x: " << k->endpt1.x << endl;
					cout << "x_int: " << x_int << "\tk->endpt2.x: " << k->endpt2.x << endl;
					cout << "y_int: " << y_int << "\tk->endpt1.y: " << k->endpt1.y << endl;
					cout << "y_int: " << y_int << "\tk->endpt2.y: " << k->endpt2.y << endl << endl;*/

					if((x_int >= k->endpt1.x) && (x_int <= k->endpt2.x) && (y_int <= k->endpt1.y) && (y_int >= k->endpt2.y))
					{
						Point new_point(x_int, y_int, k->endpt1.z, 0, false);
						intersect_pts.insert(new_point);
						cout << "Point added in" << endl;
					}
				}
				else
				{
					cout << "IN HERE, INSTEAD...." << endl;
					/*//for debug...
					//cout << "x_int: " << x_int << "\tk->endpt1.x: " << k->endpt1.x << endl;
					//cout << "x_int: " << x_int << "\tk->endpt2.x: " << k->endpt2.x << endl;
					//cout << "y_int: " << y_int << "\tk->endpt1.y: " << k->endpt1.y << endl;
					//cout << "y_int: " << y_int << "\tk->endpt2.y: " << k->endpt2.y << endl << endl;*/

					//swap endpoints 1 and 2...
					//since I don't want to write swap for the Point class
					//we'll do it using temps...
					Point temp_1(k->endpt2.x, k->endpt2.y, k->endpt2.z, 0, false);
					Point temp_2(k->endpt1.x, k->endpt1.y, k->endpt1.z, 0, false);
					
					//if((x_int <= k->endpt1.x) && (x_int >= k->endpt2.x) && (y_int <= k->endpt1.y) && (y_int >= k->endpt2.y))
					//using the one below for now
					if((x_int >= temp_1.x) && (x_int <= temp_2.x) && (y_int <= temp_1.y) && (y_int >= temp_2.y))
					{
						Point new_point(x_int, y_int, k->endpt1.z, 0, false);
						intersect_pts.insert(new_point);
						cout << "Another point added in" << endl;
					}
				}
			}
		}
	}
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

void snap(Bridge & best_bridge, set<Point> & points_supported_by_bridge)
//void snap(Bridge &best_bridge, set<Point> &points_supported_by_bridge, vector<Pillar> &pillars, vector<Cube_Primitive> &cubes, double dist_to_obj_above)
{
	//removes points supported by bridge from active elements set
	//puts endpoints of bridge into active elements set
	//but these do not get anchoring segments because the bridge is as long as it's going to be(?)
	//which can be taken care of with an if stmt in create_anchoring_segments as to
	//whether or not the endpt is open, if it is, then it gets anchoring segment, if not, then no anchoring segment
	//also creates pillars to points above that are supported h(pt) to h(bridge)
	//once both ends of bridge are snapped, creates 'cube primitive' scad primitive 

	//get points supported by bridge & create pillars
	for(auto i = points_supported_by_bridge.begin(); i != points_supported_by_bridge.end(); i++)
	{
		//i->print_coords(cout);
		//Pillar new_pillar = make_pillar(*i, dist_to_obj_above);
		//pillars.push_back(new_pillar);
		//new_pillar.print_all(cout);
	}

	//for testing...
	best_bridge.pt1_open = false;
	best_bridge.pt2_open = false;

	//get endpoints of best_bridge & create 'cube' if both endpoints are closed
	if(!best_bridge.pt1_open && !best_bridge.pt2_open)
	{
		//Cube_Primitive new_cube = make_cube_primitive(best_bridge.p1, best_bridge.p2);
		//cubes.push_back(new_cube);
	}
}

//input: set of points that need support from getPoints
//output: bridge structure
void generate_scaffolding(set<Point> points_needing_support)
{

}


//set<Segment> anchoring_segments;
//set<SweepDirections> sweep_directions;

//structures needed: //ALL DONE, I THINK
//set_of_active_elements --DONE set<Point> active_points
//set_of_anchoring_segments --DONE set<Anchring_Segments> segments
//queue_of_events, which is a queue_of_points  --DONE queue<Point> events
//events needs to be refactored as a PRIORITY_QUEUE --DONE priority_queue<Point> events
//set_of_bridges --DONE set<Bridges> bridges
//set_of_sweep_directions (equations of sweep plane?) --DONE vector<double> slope_of_sweep

//functions needed:
//create_anchoring_segments --DONE
//create_events --DONE
//union_sets --DONE
//select_bridge ******************************TODO******************************
//difference_sets --DONE
//snap_element_and_bridge *******************************TODO**********************************
//check_collisions ***********************************TODO****************************
//input: model, bridge, each connector to the elements above it
//A, i, j, z
//A is points (above) supported by bridge
//i is P[i] anchoring segment
//j is P[j] anchoring segment
//z is layer
//calculate_gain_and_score ********************************TODO**************************
//input: bridge created from A, i, j, z
//A is points (above) supported by bridge, use size() of function
//i is P[i] anchoring segment
//j is P[j] anchoring segment
//z is layer
//call calculate_score, call calculate_gain
//add_supported *******************************TODO********************************
//move P[j] into supported_elements container

//input: a set of points that require supports AND d, the number of sweep directions
//output: a valid bridge structure

//initialize set of active elements with set of points that require supports

//EVENTS ARE END POINTS & INTERSECTIONS OF ALL ANCHORING SEGMENTS - THE INTERSECTION BEING COMPUTED AFTER PROJECTION ONTO THE XY-PLANE
//THAT IS, THEY *****IGNORE***** THE Z COORDINATE

/*
while(true)
{
	best_bridge = 0;  //bestBridge is null
	for(i = 0; i < d-1; i++) //for each sweep plane
	{
		set_of_anchoring_segments (S) = create_anchoring_segments(active_elements (E), i_direction(i))
		set of segments_crossing_sweep_plane (P) = 0; //empty set
		queue_of_events (Q) = events_from_create_anchoring_segments events(S), sorted by increasing X
		while(queue_of_events is not empty)
		{
			e = pop_queue //leftmost event
			for each anchoring_segment in set_of_anchoring_segments
				starting in e do
				segements_crossing_sweep_plane = segments_crossing_sweep_plane union with s
			selected = selectBridge(segments_crossing_sweep_plane)
			if(score_of_selected_bridge > score_of_best_bridge)
			{
				bestBridge = selected;
			}
			for each anchoring_segment in set_of_anchoring_segments
				do segments_crossing_sweep_plane = segments_crossing_sweep_plane less anchoring_segment
		}
	}
	if(best_bridge = 0)
	{
		return;
	}
	C = set of elements supported by bestBridge
	for each c in C
	{
		snap(c, bestBridge)
	}
	active_elements = active_elements unioned with bestBridge
}*/
