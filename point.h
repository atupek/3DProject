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
    double extrude_amt;
    bool perimeter;
    double z;

    //constructors
    Point();
    Point(double _x, double _y);
    Point(double _x, double _y, double _extrude_amt);
    Point(double _x, double _y, double _extrude_amt, bool _perim);
    Point(double _x, double _y, double _z, double _extrude_amt, bool _perim);
    Point(const Point &p);
    
    //accessor functions
    double get_x()const;
    double get_y() const;
    double get_extrude() const;
    bool get_perim() const;
    
    //mutator functions
    void set_x(double _x);
    void set_y(double _y);
    void set_z(double _z);
    void set_extrude(double _extrude_amt);
    void set_perim(bool _perim);

    //function for sending point to ostream for printing
    void print_coords(ostream &)const;
    void print_all(ostream &)const;
    void print_coords_with_z(ostream & os) const;

    friend ostream & operator<<(ostream &, const Point &);
    friend bool operator < (const Point & lhs, const Point & rhs);
    friend bool operator > (const Point & lhs, const Point & rhs);

};

#endif //Point_Included