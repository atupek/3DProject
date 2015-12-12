//events.h
#ifndef Point_Event_Included
#define Point_Event_Included

#include "point.h"
#include "anchoring_segment.h"
#include <vector>
using std::vector;
#include <set>
using std::set;

//Bridges are represented as two endpoints & distance between them, as well as a height, and whether or not each endpoint is open or closed

class Event
{
public:
	Point p1;
	vector<Anchoring_Segment> event_segments;
	
	Event();
	Event(Point _p1);
    Event(Point _p1, vector<Anchoring_Segment> _event_segments);

    //function for sending bridge to ostream for printing
    void print_event_members(ostream &)const;

    friend ostream & operator<<(ostream &, const Event &);
    friend bool operator < (const Event & lhs, const Event & rhs);
    friend bool operator > (const Event & lhs, const Event & rhs);
};

#endif //Point_Event_Included