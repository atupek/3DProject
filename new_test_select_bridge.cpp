//test_seclect_bridge.cpp
//basic tester for select_bridge functions
#include "select_bridge.h"
#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include "get_points/point.h"
//#include "cube_primitive.h"
//#include "pillar.h"
#include "event.h"
#include <limits> //for infinity
#include "sweep_line.h"
using std::make_pair;
#include "generate_scaffolding.h"

//NOTE:
//TEST NEEDS TO BE REWRITTEN TO TAKE VECTOR OF SWEEP LINES...
//WHICH I REALLY DON'T FEEL LIKE DOING SINCE THE CODE HAS BEEN VERIFIED VIA SPREADSHEET...

double test_sweep_slope = std::numeric_limits<double>::infinity();

/*vector<Sweep_line> test_sweep_lines;

void make_sweep_lines()
{
	//Points & Anchoring Segments
	Point p0(11, 15, 20);
	Anchoring_Segment seg0(p0, test_sweep_slope, false);
	Anchoring_Segment seg1(p0, test_sweep_slope, true);

	Point p1(18, 24, 17);
	Anchoring_Segment seg2(p1, test_sweep_slope, false);
	Anchoring_Segment seg3(p1, test_sweep_slope, true);

	Point p2(19, 9, 15);
	Anchoring_Segment seg4(p2, test_sweep_slope, false);
	Anchoring_Segment seg5(p2, test_sweep_slope, true);

	Point p3(25, 20, 18);
	Anchoring_Segment seg6(p3, test_sweep_slope, false);
	Anchoring_Segment seg7(p3, test_sweep_slope, true);

	//make intersect pts for sweep_line0
	Point ip0(11, 24, 17);
	Point ip1(11, 9, 15);
	Point ip2(11, 20, 18);
	
	//make pairs for sweep_line0
	point_seg_pair pair0 = make_pair(p0, seg0);
	point_seg_pair pair1 = make_pair(p0, seg1);
	point_seg_pair pair2 = make_pair(ip0, seg3);
	point_seg_pair pair3 = make_pair(ip1, seg5);
	point_seg_pair pair4 = make_pair(ip2, seg7);

	//make sweep_line0
	Sweep_line line0;
	line0.intersected_points.push_back(pair0);
	line0.intersected_points.push_back(pair1);
	line0.intersected_points.push_back(pair2);
	line0.intersected_points.push_back(pair3);
	line0.intersected_points.push_back(pair4);

	test_sweep_lines.push_back(line0);

	//make intersect pts for sweep_line1
	Point ip3(18, 15, 20);
	Point ip4(18, 9, 15);
	Point ip5(18, 20, 18);

	//make pairs for sweep_line1
	point_seg_pair pair5 = make_pair(ip3, seg0);
	point_seg_pair pair6 = make_pair(p1, seg2);
	point_seg_pair pair7 = make_pair(p1, seg3);
	point_seg_pair pair8 = make_pair(ip4, seg5);
	point_seg_pair pair9 = make_pair(ip5, seg7);

	Sweep_line line1;
	line1.intersected_points.push_back(pair5);
	line1.intersected_points.push_back(pair6);
	line1.intersected_points.push_back(pair7);
	line1.intersected_points.push_back(pair8);
	line1.intersected_points.push_back(pair9);

	test_sweep_lines.push_back(line1);

	//make intersect pts for sweep_line2
	Point ip6(19, 15, 20);
	Point ip7(19, 24, 17);
	Point ip8(19, 20, 18);

	//make pairs for sweep_line2
	point_seg_pair pair10 = make_pair(ip6, seg0);
	point_seg_pair pair11 = make_pair(ip7, seg2);
	point_seg_pair pair12 = make_pair(p2, seg4);
	point_seg_pair pair13 = make_pair(p2, seg5);
	point_seg_pair pair14 = make_pair(ip8, seg7);

	Sweep_line line2;
	line2.intersected_points.push_back(pair10);
	line2.intersected_points.push_back(pair11);
	line2.intersected_points.push_back(pair12);
	line2.intersected_points.push_back(pair13);
	line2.intersected_points.push_back(pair14);

	test_sweep_lines.push_back(line2);

	//make intersect pts for sweep_line3
	Point ip9(25, 15, 20);
	Point ip10(25, 24, 17);
	Point ip11(25, 9, 15);

	//make pairs for sweep_line3
	point_seg_pair pair15 = make_pair(ip9, seg0);
	point_seg_pair pair16 = make_pair(ip10, seg2);
	point_seg_pair pair17 = make_pair(ip11, seg4);
	point_seg_pair pair18 = make_pair(p3, seg6);
	point_seg_pair pair19 = make_pair(p3, seg7);

	Sweep_line line3;
	line3.intersected_points.push_back(pair15);
	line3.intersected_points.push_back(pair16);
	line3.intersected_points.push_back(pair17);
	line3.intersected_points.push_back(pair18);
	line3.intersected_points.push_back(pair19);

	test_sweep_lines.push_back(line3);

}*/

set<Point> test_pts;

void make_point_set()
{
	Point p0(11, 15, 20);
	Point p1(18, 24, 17);
	Point p2(19, 9, 15);
	Point p3(25, 20, 18);
	test_pts.insert(p0);
	test_pts.insert(p1);
	test_pts.insert(p2);
	test_pts.insert(p3);
}

vector<double> test_slope_vec;
void make_sweep_vector()
{
	test_slope_vec.push_back(0.0);// horizontal gets nan as a result b/c divide by zero...need a condition for this TODO
	test_slope_vec.push_back(1.0); //45 degrees
	test_slope_vec.push_back(2.0);
	test_slope_vec.push_back(3.0);
	test_slope_vec.push_back(std::numeric_limits<double>::infinity()); // vertical...net a condition for this TODO
	//think I've taken care of the two TODO's above...
}

int main()
{
	Bridge test_bridge;

	//make_sweep_lines();

	/*//for debug
	cout << "Toy Segment Set: " << endl;
	for(auto i = test_sweep_lines.begin(); i != test_sweep_lines.end(); i++)
	{
		i->print_sweep_line_members(cout);
	}*/
	
	make_point_set();

	set<Bridge> test_bridges;
	set<Anchoring_Segment> test_segments;

	make_sweep_vector();

 	create_anchoring_segments(test_pts, test_bridges, test_segments, test_slope_vec, 4);

 	set<Event> test_events;
 	create_events(test_segments, test_events);

 	vector<Sweep_line> test_sweep_lines;
 	find_intersections(test_events, test_slope_vec, 4, test_sweep_lines);

 	test_bridge = select_bridge(test_sweep_lines, test_slope_vec[4]);

	cout << "Let's see what the toy set produces: " << endl;
	test_bridge.print_bridge_members(cout);
	cout << endl;
	cout << "Point 1 with z: " << endl;
	test_bridge.p1.print_coords_with_z(cout);
	cout << endl;
	cout << "Point 2 with z: " << endl;
	test_bridge.p2.print_coords_with_z(cout);
	cout << endl;

/*
	for(auto i = the_best_bridge.supported_points.begin(); i != the_best_bridge.supported_points.end(); i++)
	{
		i->print_coords_with_z(cout);
	}*/

/*  //for debug
	cout << "***************************Testing calc_dist************************" << endl;
	double dist0 = calc_dist(0, 0, 0, 5);
	cout << "dist 0: " << dist0 << " expected 5" << endl;
	double dist1 = calc_dist(0, 0, 5, 0);
	cout << "dist 1: " << dist1 << " expected 5" << endl;
	double dist2 = calc_dist(0, 5, 0, 0);
	cout << "dist 2: " << dist2 << " expected 5" << endl;
	double dist3 = calc_dist(5, 0, 0, 0);
	cout << "dist 3: " << dist3 << " expected 5" << endl;*/

	return 0;
}