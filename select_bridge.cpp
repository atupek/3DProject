#include "select_bridge.h"
#include <limits> //for inf

double calculate_lmax(double height, double length)
{
	//1.0 + 2.0 = 3.0
	return height + length;
}

//G(b) = (k-2)h(b)-w(b)
//w(b) = length of bridge
//h(b) = height of bridge
//k = # elements supported
double calculate_gain(double height, double length, int num_elements)
{
	//(4 - 2) * 1.0 - 2.0 = 0
	return((num_elements-2)*height) - length;
}

//in cases where bridge extremities are above the object, using free length of each vertical pillar instead of the bridge height
//h1 & h2 are the heights of the pillars before reaching the object.
//DO I EVEN WANT TO USE THIS?
double calculate_gain_above_object(int num_elements, double height, double length, double h1, double h2)
{
	//4 * 1.0 -2.0 -4.0 - 2.0 = -4.0
	return num_elements * height - h1 -h2 - length;
}

//S(b) = G(b) - k * lmax(b)
//lmax(b) is the max length of structure connecting an element above to the bridge
double calculate_score(double gain, int num_elements, double lmax)
{
	//0 - 4 * 3 = -12.0
	return gain - num_elements * lmax;
}

double calculate_distance(double x1, double y1, double x2, double y2)
{
	double x_squared = pow(x2 - x1, 2);
	double y_squared = pow(y2 - y1, 2);
	return sqrt(x_squared + y_squared);
}

void check_collision()
{
	//I HAVE NO IDEA HOW TO DO THIS
	//checking for collisions between bridge and model?
	
}

//input to select_bridge set of segments (P) intersecting sweep plane at the current event
//returns bestBridge
//Container (C) initialized with segments that intesect sweep plane
//Z, sort all segments in container (C) by Z
//for each level (z)
//	for i=0 to size(Z)-1
//		Points_supported_by_bridge (A) <- 0 //initialize to empty set
//		for j=i to size(Z) -1
//			if distance from C[i] to C[j] <= max_distance
//				put C[j] into Points_supported_by_bridge
//				if there are collisions between points_supported_by_bridge, i, j, z then break
//				get bridge gain & score for points_supported_by_bridge, i, j, z
//				if gain>0 and score > bestScore then bestBridge = currentBridge
//return bestBridge
void select_bridge(set<Anchoring_Segment> segment)
{
	set<Point> pts_supported_by_best_bridge;
	cout << "POINTS SUPPORTED BY BEST BRIDGE SIZE: " << pts_supported_by_best_bridge.size() << endl;
	double neg_inf(-std::numeric_limits<double>::infinity());
	Bridge best_bridge;
	double best_score = neg_inf;
	cout << "BEST BRIDGE BEFORE: " << endl;
	best_bridge.print_bridge_pts_height(cout);
	double max_dist = 15.0;
	//go through set of segments, measure distance between segments
	//segment has intersected points vector
	for(auto i = segment.begin(); i!= segment.end(); i++)
	{
		set<Point> supported_by_bridge;
		for(auto k = 0; k < i->intersected_points.size(); k++)
		{
			for(auto m = k+1; m < i->intersected_points.size(); m++)
			{
				//cout  << "calculating distance between: " << i->intersected_points[k].x << ", " << i->intersected_points[k].y << " & " <<
															// i->intersected_points[m].x << ", " << i->intersected_points[m].y << endl;
				double dist = calculate_distance(i->intersected_points[k].x, i->intersected_points[k].y, i->intersected_points[m].x, i->intersected_points[m].y);
				//cout << "distance: " << dist << endl;
				if(dist <= max_dist)
				{
					//TODO: this should be temp, cleared & refilled for each until we get the best bridge
					supported_by_bridge.insert(i->intersected_points[m]);
					//if there are collisions, break
					check_collision();
					//evaluate bridge to get gain & score
					//calculate_gain(height, length, num_elements)
					//Bridge(Point _p1, Point _p2, double _length, double _height, bool _pt1_open, bool _pt2_open);
					//bridge's endpoints are supported_by_bridge[0], supported_by_bridge[size-1]
					//length can be computed by calculating distance between the two endpoints
					//height is given by current z (which is not yet in this algorithm?!?) -- TODO
					//both points are open to begin with, set closed by snap function
					Point endpt1 = *supported_by_bridge.begin();
					Point endpt2 = *supported_by_bridge.rbegin(); //iterator to last element
					//if pt1 == pt2 then do nothing...which is also if the size of supported_by_bridge ==1, right?
					if(supported_by_bridge.size() != 1)
					{
						//Bridge current_bridge(endpt1, endpt2, 10); //TODO: bridge_height = 10 for now, needs to be fixed
						Bridge current_bridge(endpt1, endpt2, endpt1.z);
						//current_bridge.print_bridge_pts_height(cout);
						//calculate_gain(double height, double length, int num_elements)
						double this_gain = calculate_gain(current_bridge.height, current_bridge.length, supported_by_bridge.size());
						//cout << "This gain: " << this_gain << endl;
						//calculate_score(double gain, int num_elements, double lmax)
						double this_lmax = calculate_lmax(current_bridge.height, current_bridge.length);
						//cout << "This lmax: " << this_lmax << endl;
						double this_score = calculate_score(this_gain, supported_by_bridge.size(), this_lmax);
						//cout << "This score: " << this_score << endl; 
						//if gain > 0 and score > best_score then
						if(this_gain > 0 && this_score > best_score)
						{
							pts_supported_by_best_bridge.clear();
							best_score = this_score;
							best_bridge = current_bridge;
							best_bridge.score = this_score;
							pts_supported_by_best_bridge = supported_by_bridge;
						}
					}
				}
			}
			//cout << "Supported by bridge size: " << supported_by_bridge.size() << endl;
			supported_by_bridge.clear();
		}
		//if that distance is less than max distance
		//add points of C[j] to supported_by_bridge
		//check for collisions between supported_by_bridge, segment[i], segment[j], z-height
		//calculate gain & score of a bridge composed of supported_by_bridge, segemnt[i], segment[j], z-height
		//if gain > 0 && score > best_score
		//best_bridge = current_bridge;
		//cout << "yep" << endl;
	}
	cout << "BEST BRIDGE AFTER: " << endl;
	//best_bridge.print_bridge_pts_height(cout);
	best_bridge.print_bridge_members(cout);
	cout << "POINTS SUPPORTED BY BEST BRIDGE SIZE: " << pts_supported_by_best_bridge.size() << endl;
	for(auto j = pts_supported_by_best_bridge.begin(); j != pts_supported_by_best_bridge.end(); j++)
	{
		j->print_coords_with_z(cout);
	}
	//return best_bridge
}




/*
	data from test_select_bridge
	double bridge_height = 1.0;
	double bridge_length = 2.0;
	int num_elements_supported = 4;
	double bridge_h1 = 2.0;
	double bridge_h2 = 4.0;*/

