//to test event class
#include "event.h"
#include "get_points/point.h"
#include "anchoring_segment.h"
#include <iostream>
using std::cout;
using std::endl;
#include <set>
using std::set;
#include "bridge.h"
#include "generate_scaffolding.h"
#include "get_points/process_layers.h"

set<Point> test_points;
set<Bridge> test_bridges;
set<Anchoring_Segment> segments;
set<Event> events;

void create_test_points()
{
	Point p1(19, 9, 15);
	Point p2(11, 15, 20);
	Point p3(25, 20, 18);
	Point p4(18, 24, 17);
	test_points.insert(p1);
	test_points.insert(p2);
	test_points.insert(p3);
	test_points.insert(p4);
}

int main()
{
	vector<double> slope_of_sweep;
	slope_of_sweep.push_back(1);
	int i = 0;
	create_test_points();

	create_anchoring_segments(test_points, test_bridges, segments, slope_of_sweep, i);

	Point test_point(1.0, 1.0, 1.0);
	Anchoring_Segment test_seg1;
	Anchoring_Segment test_seg2;
	vector<Anchoring_Segment> test_segs;
	test_segs.push_back(test_seg1);
	test_segs.push_back(test_seg2);
	Event test_event(test_point, test_segs);
	cout << "Test event size before: " << test_event.event_segments.size() << endl;

	set<Event> test_events;
	test_events.insert(test_event);

	for(auto i = test_events.begin(); i != test_events.end(); i++)
	{
		cout << "test event segment size inside: " << endl;
		cout << i->event_segments.size() << endl;
	}

	//weird.  so when I iterate through, it doesn't work, but when I just add it, it does work
	/*for(auto i = test_events.begin(); i != test_events.end(); i++)
	{
		Anchoring_Segment test_seg;
		i->event_segments.push_back(test_seg);
	}*/

	//weird.  so when I iterate through, it doesn't work, but when I just add it, it does work
	Anchoring_Segment test_seg3;
	test_event.event_segments.push_back(test_seg3);
	cout << "Test event size after: " << test_event.event_segments.size() << endl;


	cout << "Event size before loop: " << events.size() << endl;
	//this is create_events from generate_scaffolding.cpp
	for(auto i = segments.begin(); i != segments.end(); i++)
	{
		Event new_event(i->endpt1);
		new_event.event_segments.push_back(*i);
		
		//trying to figure out why stuff doesn't work...
		//Anchoring_Segment this_seg;
		//new_event.event_segments.push_back(this_seg);
		
		auto set_it = events.find(new_event);
		if(set_it != events.end()) // if it's already in event set, add segment to its vector of segments
		{
			cout << "event found" << endl;
			//cout << "Event segment size: " << set_it->event_segments.size() << endl;
			//can't change items in a set, so have to create a new one
			Event changed_event(*set_it); //added this line
			//and then erase the old one
			events.erase(*set_it);//added this line
			cout << "Event segment size before adding new segment: " << changed_event.event_segments.size() << endl;
			//and add the segment to the new one
			changed_event.event_segments.push_back(*i);//added this line
			cout << "Event segment size after adding new segment: " << changed_event.event_segments.size() << endl;
			//and then insert the new one into the set
			events.insert(changed_event);//added this line
		}
		else // if it's not already in event set, then add it
		{
			cout << "new event added" << endl;
			events.insert(new_event);
		}
	}

	cout << "Event size after loop: " << events.size() << endl;
	cout << "done with test" << endl;
	return 0;
}