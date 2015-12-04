//generate_scaffolding.cpp
//this is algorithm 2 from Bridging the Gap
#include "generate_scaffolding.h"
#include "select_bridge.h"
#include "bridge.h"
#include <limits> //for infinity

set<Point> active_points;
set<Bridge> active_bridges;
set<Bridge> bridges;

vector<double> slope_of_sweep;
double inf = std::numeric_limits<double>::infinity();

//for testing...
void make_point_set()
{
	for(int i = 0; i<10; i++)
	{
		Point new_point(i, i, i+10, true);
		active_points.insert(new_point);
	}
}

void make_sweep_vector()
{
	slope_of_sweep.push_back(0.0); // horizontal gets nan as a result b/c divide by zero...need a condition for this TODO
	slope_of_sweep.push_back(1.0); //45 degrees
	slope_of_sweep.push_back(inf); // vertical
}

int main()
{
	int i = 1;
	make_point_set();
	make_sweep_vector();
	create_anchoring_segments(active_points, active_bridges, slope_of_sweep, i);
	
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
