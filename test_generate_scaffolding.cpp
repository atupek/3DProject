//test program for generate scaffolding
#include "generate_scaffolding.h"
#include "select_bridge.h"
#include "bridge.h"
#include <limits> //for infinity
#include "event.h"
#include <fstream>
using std::ifstream;

//****************comment out to try from file....
//set<Point> active_points; //this is what is sent from algorithm 1: GetPoints
set<Bridge> active_bridges; //this will be added to when we have bridges returned from algorithm 3: SelectBridge

set<Anchoring_Segment> segments;

set<Event> active_events;

vector<double> slope_of_sweep;
double inf = std::numeric_limits<double>::infinity();

//******************comment out to try from file...
/*void make_point_set()
{
	//Point p0(1, 1, 1);
	Point p1(19, 9, 15);
	Point p2(11, 15, 20);
	Point p3(25, 20, 18);
	Point p4(18, 24, 17);
	//active_points.insert(p0);
	active_points.insert(p1);
	active_points.insert(p2);
	active_points.insert(p3);
	active_points.insert(p4);
}*/

void make_sweep_vector()
{
	slope_of_sweep.push_back(0.0);// horizontal gets nan as a result b/c divide by zero...need a condition for this TODO
	slope_of_sweep.push_back(1.0); //45 degrees
	slope_of_sweep.push_back(2.0);
	slope_of_sweep.push_back(3.0);
	slope_of_sweep.push_back(inf); // vertical...net a condition for this TODO
	//think I've taken care of the two TODO's above...
}

set<Point> get_pts_from_file()
{
	set<Point> pts_to_run;
	ifstream inFile("get_points/gridded_points_to_support.txt");
	//ifstream inFile("get_points/points_to_support.txt"); //for non-sparse pillars
	string line;
	string delim = ", ";
	while(!inFile.eof()) //important that pts_to_support doesn't end with an empty newline...
	{
		getline(inFile, line);
		string x_token = line.substr(0, line.find(delim));
		//cout << "x_token: " << x_token << endl;
		double x_value = stod(x_token);
		string substr_no_x = line.substr(line.find(delim)+2, line.find("\n"));
		//cout << "substr no x: " << substr_no_x << endl;
		string y_token = substr_no_x.substr(0, line.find(delim));
		//cout << "y_token: " << y_token << endl;
		double y_value = stod(y_token);
		string z_token = substr_no_x.substr(line.find(delim)+2, line.find("\n"));
		//cout << "z_token: " << z_token << endl;
		double z_value = stod(z_token);
		Point new_pt(x_value/2, y_value/2, z_value); //div by 2 since we multiplied by 2 originally
		pts_to_run.insert(new_pt);
	}
	return pts_to_run;
}

int main()
{
	//insert header info to scad file
	setup_scad();
	
	/*//for bitmap debug
	bitmap_image test_image(200, 200);
	test_image.set_all_channels(255,255,255); //set background to white
	image_drawer draw(test_image);*/

	set<Point> points_for_alg3;
	set<Anchoring_Segment> segments_for_alg3;
	vector<Sweep_line> sweep_line_vec;

	//******************SWAP TO GET FROM FILE INSTEAD OF TEST SET...
	set<Point> active_points = get_pts_from_file();
	//cout << "NUMBER OF POINTS: " << active_points.size() << endl;
	//**********comment out make_pt_set()
	//make_point_set();

	//note: find_intersections gets 5 points for sweep_slope = inf, 2, 1, but not for 0 or 3...
	//int outer_loop_index = 0;

	output_pts_to_support_to_scad(active_points, "Orchid");

	//for just dropped pillars...
	//send_remaining_points_to_scad(active_points);

	make_sweep_vector();
	for(auto outer_loop_index = 0; outer_loop_index < slope_of_sweep.size(); outer_loop_index++)
	{
		cout << "outer_loop_index: " << outer_loop_index << endl;
		create_anchoring_segments(active_points, active_bridges, segments, slope_of_sweep, outer_loop_index);

		/*//for debug: print members of anchoring segments:
		cout << "***********************ANCHORING SEGMENTS: **************************" << endl;
		for(auto i = segments.begin(); i != segments.end(); i++)
		{
			i->print_coords(cout);
		}*/

		//cout << "Anchoring segments size: "  << segments.size() << endl;
		//create events from anchoring segments

		create_events(segments, active_events);
		
		//cout << "Events size: " << active_events.size() << endl;

		//find intersections between sweep plane and anchoring segments at each event(which is a point)
		//these intersections are the points sent to select bridge

		/*//for debug, print event info:
		cout << "*******************************EVENT MEMBERS: ***************************" << endl;
		for(auto i = active_events.begin(); i != active_events.end(); i++)
		{
			i->print_event_members(cout);
		}*/

		//cout << "points for alg3 size: " << points_for_alg3.size() << endl;

		find_intersections(active_events, slope_of_sweep, outer_loop_index, sweep_line_vec);

		/*
		cout << "points for alg3 after intersections size: " << points_for_alg3.size() << endl;

		//for debug
		cout << "points being sent to alg3: " << endl;
		for(auto i = points_for_alg3.begin(); i != points_for_alg3.end(); i++)
		{
			i->print_coords(cout);
		}*/

		/*//for debug
		cout << "*****************************SEGMENT MEMBERS: ******************************" << endl;
		cout << "segments for al3 size: " << segments_for_alg3.size() << endl;
		cout << "segments for alg3:" << endl;
		for(auto i = segments_for_alg3.begin(); i != segments_for_alg3.end(); i++)
		{
			i->print_coords(cout);
			cout << "Of size: " << i->intersected_points.size() << endl;
			i->print_intersect_pts(cout);
		}*/

		Bridge temp_bridge;
		Bridge the_best_bridge;
		//the_best_bridge = select_bridge(segments_for_alg3, slope_of_sweep[i]);
		//the_best_bridge = select_bridge_sweep_line(sweep_line_vec, slope_of_sweep[i]);
		//the_best_bridge = select_bridge(sweep_line_vec, slope_of_sweep[outer_loop_index]);
		temp_bridge = select_bridge(sweep_line_vec, slope_of_sweep[outer_loop_index]);
		if(temp_bridge.score > the_best_bridge.score)
		{
			the_best_bridge = temp_bridge;
		}

		//for debug
		cout << "*******************RESULTS*********************" <<endl;
		cout << "Let's see what the segment set produced: " << endl;
		the_best_bridge.print_bridge_members(cout);
		cout << endl;
		cout << "Point 1 with z: " << endl;
		the_best_bridge.p1.print_coords_with_z(cout);
		cout << endl;
		cout << "Point 2 with z: " << endl;
		the_best_bridge.p2.print_coords_with_z(cout);
		cout << endl;
		cout << "Supported points: " << endl;
		for(auto j = the_best_bridge.supported_points.begin(); j != the_best_bridge.supported_points.end(); j++)
		{
			j->print_coords_with_z(cout);
		}

		cout << "TIME TO SNAP..." << endl;
		//snap(the_best_bridge, active_points);
		cout << "snapped..." << endl;
	}

	//drop pillars from all remaining points that need support
	send_remaining_points_to_scad(active_points);
	return 0;
}
