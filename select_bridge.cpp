#include "select_bridge.h"

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
	//go through set of segments, measure distance between segments
	//segment has intersected points vector
	//cout << "num elements in segment: " << segment.size() << endl;
	for(auto i = segment.begin(); i!= segment.end(); i++)
	{
		set<Point> supported_by_bridge;
		//cout << "in i loop" << endl;
		for(auto j = i; j != segment.end(); j++)
		{
			//cout << "in j loop" << endl;
			//calculate distance from segment[i] to segment[j]
			//cout << "num elements in i intersected points: " << i->intersected_points.size() << endl;
			
			//for(auto k = i->intersected_points.begin(); k!= i->intersected_points.end(); k++)
			for(auto k = 0; k < i->intersected_points.size()-1; k++)
			{
				//cout << "in k loop" << endl;
				//cout << "num elements in j intersected points: " << j->intersected_points.size() << endl;
				
				//for(auto m = j->intersected_points.begin(); m != j->intersected_points.end(); m++)
				for(auto m = k; m < i->intersected_points.size(); m++)
				{
					//cout << "in m loop" << endl;
					//cout << "calculating distance between: " << k->x << ", " << k->y << " & " << m->x << ", " << m->y << endl;
					//double dist = calculate_distance(k->x, k->y, m->x, m->y);
					
					cout  << "calculating distance betwen: " << i->intersected_points[k].x << ", " << i->intersected_points[k].y << " & " <<
							i->intersected_points[m].x << ", " << i->intersected_points[m].y << endl;
					double dist = calculate_distance(i->intersected_points[k].x, i->intersected_points[k].y, i->intersected_points[m].x, i->intersected_points[m].y);
					cout << "distance: " << dist << endl;
				}
			}
			//if that distance is less than max distance
			//add points of C[j] to supported_by_bridge
			//check for collisions between supported_by_bridge, segment[i], segment[j], z-height
			//calculate gain & score of a bridge composed of supported_by_bridge, segemnt[i], segment[j], z-height
			//if gain > 0 && score > best_score
			//best_bridge = current_bridge;
			//cout << "yep" << endl;
		}
	}

}




/*
	data from test_select_bridge
	double bridge_height = 1.0;
	double bridge_length = 2.0;
	int num_elements_supported = 4;
	double bridge_h1 = 2.0;
	double bridge_h2 = 4.0;*/

