//pillar class for project

#include "pillar.h"

//default constructor
Pillar::Pillar()
{
    x=0.0;
    y=0.0;
    z1=0.0;
    z2=0.0;
}

Pillar::Pillar(double _x, double _y, double _z1, double _z2)
{
    x=_x;
    y=_y;
    z1=_z1;
    z2=_z2;
}

//copy constructor
Pillar::Pillar(const Pillar &plr)
{
    x=plr.x;
    y=plr.y;
    z1=plr.z1;
    z2=plr.z2;
}

//overloaded << operator
ostream & operator << (ostream & os, const Pillar & obj)
{
    os << obj;
    return os;
}

bool operator < (const Pillar & lhs, const Pillar & rhs)
{
    return lhs.x < rhs.x;
}

bool operator > (const Pillar & lhs, const Pillar & rhs)
{
    return lhs.x > rhs.x;
}

//print function to print all point info
void Pillar::print_all(ostream & os) const
{
	os  << x << ", " << y << " from: " << z1 << " to " << z2 << endl;
}
