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
			Point new_point(i, i, .8*i, i+10, true);
			active_points.insert(new_point);
		}
		else
		{
			Point new_point(i, i, .8, i+10, true);
			active_points.insert(new_point);
		}
	}
}*/

void make_point_set()
{
	Point p1(19, 9, 15, 0, false);
	Point p2(11, 15, 20, 0, false);
	Point p3(25, 20, 18, 0, false);
	Point p4(18, 24, 17, 0, false);
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
			Point new_point(i, i, .8*i, i+10, true);
			active_points_vec.push_back(new_point);
		}
		else
		{
			Point new_point(i, i, .8, i+10, true);
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
	set<Point> points_for_alg3;
	int i = 2;
	make_point_set();
	//make_point_vector();
	make_sweep_vector();
	create_anchoring_segments(active_points, active_bridges, segments, slope_of_sweep, i);
	//cout << "Anchoring segments size: "  << segments.size() << endl;
	//create events from anchoring segments

	create_events(segments, active_events);
	
	cout << "Events size: " << active_events.size() << endl;

	//find intersections between sweep plane and anchoring segments at each event(which is a point)
	//these intersections are the points sent to select bridge

	cout << "points for alg3 size: " << points_for_alg3.size() << endl;

	find_intersections(active_events, slope_of_sweep, i, points_for_alg3);

	cout << "points for alg3 after intersections size: " << points_for_alg3.size() << endl;

	/*//for debug
	cout << "points being sent to alg3: " << endl;
	for(auto i = points_for_alg3.begin(); i != points_for_alg3.end(); i++)
	{
		i->print_coords(cout);
	}*/

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
