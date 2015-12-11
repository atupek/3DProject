//generate_scaffolding.cpp
//this is algorithm 2 from Bridging the Gap
#include "generate_scaffolding.h"
#include "select_bridge.h"
#include "bridge.h"
#include <limits> //for infinity
#include "pillar.h"
#include "cube_primitive.h"
#include "event.h"

set<Anchoring_Segment> segments;
//queue<Point> events;
priority_queue<Point, vector<Point>, std::greater<Point> > events;
priority_queue<Event, vector<Event>, std::greater<Event> > new_events;
//for testing union_sets:
set<Anchoring_Segment> test_seg;

//vector<Event> new_events;

//takes set of points and direction
//creates anchoring segements for each point
//and then creates events for each segment, inserting events into priority queue
/*Event(Point _p1, vector<Anchoring_Segment> _event_segments)
p1 = _p1;
event_segments = _event_segments;  */
void new_create_events(Point pt, double slope)
{
	vector<Anchoring_Segment> this_points_segments;
	cout << "new event from point..." << endl;
	Anchoring_Segment new_segment(pt, slope, true);
	this_points_segments.push_back(new_segment);

	Anchoring_Segment new_segment1(pt, slope, false);
	this_points_segments.push_back(new_segment1);

	Event this_event(pt, this_points_segments);
	new_events.push(this_event);
}
/*
//creates anchoring segment
void draw_line(Point pt, double slope)
{
	//line should be orthogonal to the slope, of a specific given distance
	//endpoints are the point passed, and a point at the positive distance
	//endpoints are the point passed, and a point at the negative distance
	//thus, creates two anchoring segments
	cout << "A LINE:" << endl;
	Anchoring_Segment new_segment(pt, slope, true);
	new_segment.print_coords(cout);
	segments.insert(new_segment);

	Anchoring_Segment new_segment1(pt, slope, false);
	new_segment1.print_coords(cout);
	segments.insert(new_segment1);

	//for testing union_sets:
	test_seg.insert(new_segment1);
}*/

void create_anchoring_segments(set<Point> &point_set, set<Bridge> &bridge_set, vector<double> &sweep_direction, int i)
{
	double plane = sweep_direction[i];
	//for each Point in point_set, create anchor segment of length max_length orthoganl to sweep_direction, centered on Point
	//for_each(point_set.begin(), point_set.end(), draw_line(sweep_direction[i]));
	for(auto i = point_set.begin(); i != point_set.end(); i++)
	{
		//draw_line(*i, plane);
		new_create_events(*i, plane);
	}
	for(auto i = bridge_set.begin(); i != bridge_set.end(); i++)
	{
		//draw_line(i->p1, plane);
		//draw_line(i->p2, plane);
		//*****************************TODO************************************************
		//should check if endpoit is open or closed before doing this, I think...
		new_create_events(i->p1, plane);
		new_create_events(i->p2, plane);
	}
}

void create_events(Anchoring_Segment _segment)
{
	//get intersecting points out of the anchoring segment
	for(auto i = _segment.intersected_points.begin(); i != _segment.intersected_points.end(); i++)
	{
		events.push(*i);
	}
	//get intersecting bridge points out of the anchoring segment
	for(auto i = _segment.intersected_bridges.begin(); i != _segment.intersected_bridges.end(); i++)
	{
		events.push(i->p1);
		events.push(i->p2);
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
}

void snap(Bridge &best_bridge, set<Point> &points_supported_by_bridge, vector<Pillar> &pillars, vector<Cube_Primitive> &cubes, double dist_to_obj_above)
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
		Pillar new_pillar = make_pillar(*i, dist_to_obj_above);
		pillars.push_back(new_pillar);
		new_pillar.print_all(cout);
	}

	//for testing...
	best_bridge.pt1_open = false;
	best_bridge.pt2_open = false;

	//get endpoints of best_bridge & create 'cube' if both endpoints are closed
	if(!best_bridge.pt1_open && !best_bridge.pt2_open)
	{
		Cube_Primitive new_cube = make_cube_primitive(best_bridge.p1, best_bridge.p2);
		cubes.push_back(new_cube);
	}
}

void generate_scaffolding(set<Point> pts_that_need_support)
{
	set<Bridge> bridges_that_need_support;
	vector<double> directions;
	Bridge best_bridge;
	//for testing, create direction & add to vector
	double test_direction = 1.0;
	directions.push_back(test_direction);
	for(auto i = 0; i < directions.size(); i++)
	{
		create_anchoring_segments(pts_that_need_support, bridges_that_need_support, directions, i);
		set<Anchoring_Segment> segments_crossing_plane_i;
		//segments_crossing_plane_i = segments;
		cout << "SEGEMENTS_CROSSING_PLANE SIZE: " << segments_crossing_plane_i.size() << endl;
		/*events should already be created...
		for(auto i = segments.begin(); i != segments.end(); i++)
		{
			create_events(*i);
		}*/
		
		//for testing, create event & add to events priority queue
		//Point test_point(1.0, 1.0, 0.0, 1.0, true);
		//events.push(test_point);
		cout << "EVENTS SIZE: " << events.size() << endl;
		
		while(!new_events.empty())
		//while(!events.empty())
		{
			//Point e = events.top(); //get value of first item
			//events.pop(); //remove item from priority queue
			Event e = new_events.top(); //get first event
			new_events.pop(); //remove item from priority queue
			for(auto j = e.event_segments.begin(); j != e.event_segments.end(); j++)
			{
				cout << "events..." << endl;
				segments_crossing_plane_i.insert(*j);
			}
			cout << "NEXT>>>" << endl;
			//cout << "events..." << endl;
			//Somehow...not quite sure how...
			//get all anchoring segments that have e as their endpoint...
			//I think I may need to create a class that has a point and a set of anchoring segments associated with that point...
			//cout << "SEGEMENTS_CROSSING_PLANE SIZE: " << segments_crossing_plane_i.size() << endl;
			/*for(auto j = segments.begin(); j != segments.end(); j++)
			{
				segments_crossing_plane_i.insert(*j);
				//cout << "segment inserted..." << endl;
			}*/
			
			Bridge selected = select_bridge(segments_crossing_plane_i);
			if(selected.score > best_bridge.score)
			{
				best_bridge = selected;
			}
			//remove segments from segments_crossing_plane_i
			//cout << "events size: " << events.size() << endl;
		}
	}
	//if best_bridge = null then return
	//set<Point> = elements supported by best_bridge
	cout << "Generating scaffolding..." << endl;
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
