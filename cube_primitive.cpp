//cube_primitive class for project

#include "cube_primitive.h"
/*
    double x1;
    double y1;
    double x2;
    double y2;
    double z_upper;
    double z_lower;*/

//default constructor
Cube_Primitive::Cube_Primitive()
{
    x1=0.0;
    y1=0.0;
    x2=0.0;
    y2=0.0;
    z_upper=0.0;
    z_lower=0.0;
}

Cube_Primitive::Cube_Primitive(double _x1, double _y1, double _x2, double _y2, double _z_upper, double _z_lower)
{
    x1=_x1;
    y1=_y1;
    x2=_x2;
    y2=_y2;
    z_upper=_z_upper;
    z_lower=_z_lower;
}

//copy constructor
Cube_Primitive::Cube_Primitive(const Cube_Primitive & cube_prim)
{
    x1=cube_prim.x1;
    y1=cube_prim.y1;
    x2=cube_prim.x2;
    y2=cube_prim.y2;
    z_upper=cube_prim.z_upper;
    z_lower=cube_prim.z_lower;
}

//overloaded << operator
ostream & operator << (ostream & os, const Cube_Primitive & obj)
{
    os << obj;
    return os;
}

bool operator < (const Cube_Primitive & lhs, const Cube_Primitive & rhs)
{
    return lhs.x1 < rhs.x1;
}

bool operator > (const Cube_Primitive & lhs, const Cube_Primitive & rhs)
{
    return lhs.x1 > rhs.x1;
}

//print function to print all point info
void Cube_Primitive::print_all(ostream & os) const
{
	os  << x1 << ", " << y1 << " to: " << x2 << ", " << y2 << " at: z= " << z_lower << " to z= " << z_upper <<endl;
}
