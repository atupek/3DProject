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

	find_intersections(active_events, slope_of_sweep, i, points_for_alg3);

	/*
	cout << "points for alg3 after intersections size: " << points_for_alg3.size() << endl;

	//for debug
	cout << "points being sent to alg3: " << endl;
	for(auto i = points_for_alg3.begin(); i != points_for_alg3.end(); i++)
	{
		i->print_coords(cout);
	}*/

	cout << "segments for alg3:" << endl;
	for(auto i = active_events.begin(); i != active_events.end(); i++)
	{
		for(auto j = i->event_segments.begin(); j!= i->event_segments.end(); j++)
		{
			j->print_coords(cout);
			j->print_intersect_pts(cout);
		}
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

	/*for(auto i = active_events.begin(); i != active_events.end(); i++)
	{
		cout << "**************************NEW EVENT:" << endl;
		i->print_event_members(cout);
	}*/

	//condense_events(active_events, condensed_events);

	//cout << "Events size after condensing: " << condensed_events.size() << endl;

	/*for(auto i = active_events.begin(); i != active_events.end(); i++)
	{
		cout << "**************************NEW EVENT:" << endl;
		i->print_event_members(cout);
	}*/

	/*for(auto i = active_events.begin(); i != active_events.end(); i++)
	{
		i->print_event_members(cout);
	}*/

	//generate_scaffolding1(active_points_vec);
	
	//THIS IS THE ONE THAT I'M CURRENTLY WORKING WITH...
	//generate_scaffolding(active_points);
	/*
	double height_difference = 5.0; //TODO, currently a hack
	
	//Point(double _x, double _y, double _z, double _extrude_amt, bool _perim);
	Point best_endpt_1(1.0, 1.0, .8, 0.0, true);
	Point best_endpt_2(9.0, 9.0, .8, 0.0, true);

	Bridge best_bridge(best_endpt_1, best_endpt_2, best_endpt_1.z);
	
	//TODO height_difference is currently a hack...
	snap(best_bridge, active_points, scad_pillars, scad_cubes, height_difference);

	cout << "Pillar vector size: " << scad_pillars.size() << endl;
	cout << "Cube_Primitive vector size: " << scad_cubes.size() << endl;*/
	//generate_scaffolding(active_points);
	
	//for testing union & difference of sets
	/*
	cout << "segments size before difference: " << segments.size() << endl;
	difference_sets(segments, test_seg);
	cout << "segments size after difference: " << segments.size() << endl;
	cout << "test_seg size before union: " << test_seg.size() << endl;
	union_sets(test_seg, segments);
	cout << "test_seg size after union: " << test_seg.size() << endl;
	difference_sets(test_seg, segments);
	cout << "test_seg size after difference: " << test_seg.size() << endl;*/

	return 0;
}
