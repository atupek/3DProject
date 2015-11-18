//point class header for project
#ifndef Point_Included
#define Point_Included

#include <iostream>
using std::ostream;
using std::cout;
using std::endl;

class Point
{
public:
    double x;
    double y;
    bool perimeter;

    //constructors
    Point();
    Point(double _x, double _y);
    Point(double _x, double _y, bool _perim);
    Point(const Point &p);
    
    //accessor functions
    double get_x()const;
    double get_y() const;
    bool get_perim() const;
    
    //mutator functions
    void set_x(double _x);
    void set_y(double _y);
    void set_perim(bool _perim);

    //function for sending point to ostream for printing
    void print_coords(ostream &)const;
    void print_all(ostream &)const;

    friend ostream & operator<<(ostream &, const Point &);

};

#endif //Point_Included