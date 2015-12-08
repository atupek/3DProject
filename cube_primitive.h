//cube primitive class header for project
#ifndef Cube_Primitive_Included
#define Cube_Primitive_Included

#include <iostream>
using std::ostream;
using std::cout;
using std::endl;

class Cube_Primitive
{
public:
    double x1;
    double y1;
    double x2;
    double y2;
    double z_upper;
    double z_lower;

    //constructors
    Cube_Primitive();
    Cube_Primitive(double _x1, double _y1, double x2, double y2, double _z_upper, double _z_lower);
    Cube_Primitive(const Cube_Primitive &cube_prim);
    
    //function for sending point to ostream for printing
    void print_all(ostream &)const;

    friend ostream & operator<<(ostream &, const Cube_Primitive &);
    friend bool operator < (const Cube_Primitive & lhs, const Cube_Primitive & rhs);
    friend bool operator > (const Cube_Primitive & lhs, const Cube_Primitive & rhs);

};

#endif //Cube_Primitive_Included