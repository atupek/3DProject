//point class for project

#include "point.h"

//default constructor
Point::Point()
{
    x=0.0;
    y=0.0;
    extrude_amt = 0.0;
    perimeter = false;
}

Point::Point(double _x, double _y)
{
    x = _x;
    y = _y;
    extrude_amt = 0.0;
    perimeter = false;
}

Point::Point(double _x, double _y, double _extrude_amt)
{
    x = _x;
    y = _y;
    extrude_amt = _extrude_amt;
    perimeter = false;
}

Point::Point(double _x, double _y, double _extrude_amt, bool _perim)
{
    x = _x;
    y = _y;
    extrude_amt = _extrude_amt;
    perimeter = _perim;
}

/*
//copy constructor
Point::Point(const Point &p)
{
    x=p.x;
    y=p.y;
    perimeter=p.perimeter;
}
*/

void Point::set_x(double _x)
{
    x = _x;
}


void Point::set_y(double _y)
{
    y = _y;
}

void Point::set_extrude(double _extrude_amt)
{
	extrude_amt = _extrude_amt;
}

void Point::set_perim(bool _perim)
{
	perimeter = _perim;
}


double Point::get_x() const 
{
    return x;
}


double Point::get_y() const
{
    return y;
}

double Point::get_extrude() const
{
	return extrude_amt;
}

bool Point::get_perim() const
{
	return perimeter;
}

//overloaded << operator
ostream & operator << (ostream & os, const Point & obj)
{
    os << obj;
    return os;
}

//print function to print point coordinates
void Point::print_coords(ostream & os) const
{
	os << x << ", " << y << endl;

}

//print function to print all point info
void Point::print_all(ostream & os) const
{
	os  << x << ", " << y << ": " << extrude_amt;
	if(perimeter == true)
	{
		os << ", perimeter" << endl;
	}
	else
		os << ", infill" << endl;

}

