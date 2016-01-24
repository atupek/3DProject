//test program for generate scaffolding
#include "generate_scaffolding.h"
#include "select_bridge.h"
#include "bridge.h"
#include <limits> //for infinity
#include "event.h"
//#include "pillar.h"
//#include "cube_primitive.h"

set<Point> active_points;
set<Bridge> active_bridges;
set<Bridge> bridges;
set<Anchoring_Segment> segments;

//vector<Pillar> scad_pillars;
//vector<Cube_Primitive> scad_cubes;
vector<Point> active_points_vec;
set<Event> active_events;
set<Event> condensed_events;

vector<double> slope_of_sweep;
double inf = std::numeric_limits<double>::infinity();

/*//for testing...
void make_point_set()
{
	for(int i = 0; i<10; i++)
	{
		if(i%2==0)
		{
			Point new_point(i, i, .8*i);
			active_points.insert(new_point);
		}
		else
		{
			Point new_point(i, i, .8);
			active_points.insert(new_point);
		}
	}
}*/

void make_point_set()
{
	Point p1(19, 9, 15);
	Point p2(11, 15, 20);
	Point p3(25, 20, 18);
	Point p4(18, 24, 17);
	active_points.insert(p1);
	active_points.insert(p2);
	active_points.insert(p3);
	active_points.insert(p4);
}

void make_point_vector()
{
	for(int i = 0; i<10; i++)
	{
		if(i%2==0)
		{
			Point new_point(i, i, .8*i);
			active_points_vec.push_back(new_point);
		}
		else
		{
			Point new_point(i, i, .8);
			active_points_vec.push_back(new_point);
		}
	}
}

void make_sweep_vector()
{
	slope_of_sweep.push_back(0.0); // horizontal gets nan as a result b/c divide by zero...need a condition for this TODO
	slope_of_sweep.push_back(1.0); //45 degrees
	slope_of_sweep.push_back(2.0);
	slope_of_sweep.push_back(inf); // vertical
}


int main()
{
	/*//for bitmap debug
	bitmap_image test_image(200, 200);
	test_image.set_all_channels(255,255,255); //set background to white
	image_drawer draw(test_image);*/

	set<Point> points_for_alg3;
	set<Anchoring_Segment> segments_for_alg3;
	int i = 1;
	make_point_set();

	//make_point_vector();
	make_sweep_vector();
	create_anchoring_segments(active_points, active_bridges, segments, slope_of_sweep, i);

	/*//for debug: print members of anchoring segments:
	cout << "SEGMENT MEMBERS: " << endl;
	for(auto i = segments.begin(); i != segments.end(); i++)
	{
		i->print_coords(cout);
	}*/

	//cout << "Anchoring segments size: "  << segments.size() << endl;
	//create events from anchoring segments

	create_events(segments, active_events);
	
	cout << "Events size: " << active_events.size() << endl;

	//find intersections between sweep plane and anchoring segments at each event(which is a point)
	//these intersections are the points sent to select bridge

	/*//for debug, print event info:
	cout << "EVENT MEMBERS: " << endl;
	for(auto i = active_events.begin(); i != active_events.end(); i++)
	{
		i->print_event_members(cout);
	}*/

	cout << "points for alg3 size: " << points_for_alg3.size() << endl;

	//find_intersections(active_events, slope_of_sweep, i, points_for_alg3);
	find_intersections(active_events, slope_of_sweep, i, segments_for_alg3);

	/*
	cout << "points for alg3 after intersections size: " << points_for_alg3.size() << endl;

	//for debug
	cout << "points being sent to alg3: " << endl;
	for(auto i = points_for_alg3.begin(); i != points_for_alg3.end(); i++)
	{
		i->print_coords(cout);
	}*/

	cout << "segments for alg3:" << endl;
	for(auto i = segments_for_alg3.begin(); i != segments_for_alg3.end(); i++)
	{
		i->print_coords(cout);
		cout << "Of size: " << i->intersected_points.size() << endl;
		i->print_intersect_pts(cout);
	}

	//*******************************WHERE I AM NOW:
	//algorithm3 takes set of anchoring segments, not set of points, so need to adjust
	//find intersections function to return set of anchoring segments, not set of points
	//HA! find_intersections returns void, but it does create a points_for_alg3 set of points
	//let's also create a segments_for_alg3 set of anchoring_segments **************WORKING ON THIS
	//algorithm 3 then returns a Bridge
	//then that bridge's score is compared to the best bridge's score
	//if it's better, then remove the anchoring segments from the set of active elements
	//then take the set of elements supported by the best bridge
	//and snap them to the bridge
	//then union those elements with the set of active elements

	return 0;
}
