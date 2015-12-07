//test program for generate scaffolding
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
		Point new_point(i, i, .8, i+10, true);
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
	
	//Point(double _x, double _y, double _z, double _extrude_amt, bool _perim);
	Point best_endpt_1(1.0, 1.0, .8, 0.0, true);
	Point best_endpt_2(9.0, 9.0, .8, 0.0, true);

	Bridge best_bridge(best_endpt_1, best_endpt_2, best_endpt_1.z);
	snap(best_bridge, active_points);
	
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
