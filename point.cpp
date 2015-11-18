//point class for project

#include "point.h"

//default constructor
Point::Point()
{
    x=0.0;
    y=0.0;
    perimeter = false;
}

//constructor for user entered width & height
Point::Point(double _x, double _y)
{
    x = _x;
    y = _y;
    perimeter = false;
}

//constructor for user entered width & height & type of box
Point::Point(double _x, double _y, bool _perim)
{
    x = _x;
    y = _y;
    perimeter = _perim;
}

//copy constructor
Point::Point(const Point &p)
{
    x=p.x;
    y=p.y;
    perimeter=p.perimeter;
}

//set function for user to set width
void Point::set_x(double _x)
{
    x = _x;
}

//set function for user to set height
void Point::set_y(double _y)
{
    y = _y;
}

void Point::set_perim(bool _perim)
{
	perimeter = _perim;
}

//get function to return width
double Point::get_x() const //return value of width
{
    return x;
}

//get function to return height
double Point::get_y() const
{
    return y;
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
void Point::print_coords(ostream &) const
{
	cout  << x << ", " << y << endl;

}

//print function to print all point info
void Point::print_all(ostream &) const
{
	cout  << x << ", " << y;
	if(perimeter == true)
	{
		cout << ", perimeter" << endl;
	}
	else
		cout << ", infill" << endl;

}

