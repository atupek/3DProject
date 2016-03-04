//events.h
#ifndef Point_Event_Included
#define Point_Event_Included

#include "get_points/point.h"
#include "anchoring_segment.h"
#include <vector>
using std::vector;
#include <set>
using std::set;

//Event is a point and its corresponding anchoring segments, of which there are two
class Event
{
public:
	Point p1;
	vector<Anchoring_Segment> event_segments;
	
	Event();
	Event(Point _p1);
    Event(Point _p1, vector<Anchoring_Segment> _event_segments);

    //function for sending event to ostream for printing
    void print_event_members(ostream &) const;

    friend ostream & operator<<(ostream &, const Event &);
    friend bool operator < (const Event & lhs, const Event & rhs);
    friend bool operator > (const Event & lhs, const Event & rhs);
};

#endif //Point_Event_Included