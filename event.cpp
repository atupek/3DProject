//event.cpp
#include "event.h"

Event::Event()
{
	Point this_point(0.0, 0.0, 0.0, 0.0, true);
	p1 = this_point;
	event_segments = {};

}

Event::Event(Point _p1)
{
	p1 = _p1;
	event_segments = {};

}

Event::Event(Point _p1, vector<Anchoring_Segment> _event_segments)
{
	p1 = _p1;
	event_segments = _event_segments;  
}

//overloaded << operator
ostream & operator << (ostream & os, const Event & obj)
{
    os << obj;
    return os;
}

bool operator < (const Event & lhs, const Event & rhs)
{
    return lhs.p1.x < rhs.p1.x;
}

bool operator > (const Event & lhs, const Event & rhs)
{
    return lhs.p1.x > rhs.p1.x;
}

//print function to print point coordinates
void Event::print_event_members(ostream & os) const
{
	os << "Point1: ";
	p1.print_coords(os);
	os << "Anchoring Segments: " << endl;
	for(auto i = event_segments.begin(); i != event_segments.end(); i++)
	{
		i->print_coords(os);
	}
}

