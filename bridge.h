//bridge.h
#ifndef Bridge_Included
#define Bridge_Included

#include "point.h"

//Bridges are represented as two endpoints & distance between them, as well as a height, and whether or not each endpoint is open or closed

class Bridge
{
public:
	Point p1;
	Point p2;
	double length;
	double height;
	bool pt1_open;
	bool pt2_open;
	double score;
	
	Bridge();
	Bridge(Point _p1, Point _p2, double _height);
    Bridge(Point _p1, Point _p2, double _length, double _height, bool _pt1_open, bool _pt2_open);

    //function for sending bridge to ostream for printing
    void print_bridge_members(ostream &)const;
    void print_bridge_pts_length(ostream & os) const;
	void print_bridge_pts_height(ostream & os) const;

    friend ostream & operator<<(ostream &, const Bridge &);
    friend bool operator < (const Bridge & lhs, const Bridge & rhs);
    friend bool operator > (const Bridge & lhs, const Bridge & rhs);
    friend bool operator == (const Bridge & lhs, const Bridge & rhs);
};

#endif //Bridge_Included