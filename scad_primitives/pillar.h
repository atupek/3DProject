//pillar class header for project
#ifndef Pillar_Included
#define Pillar_Included

#include <iostream>
using std::ostream;
using std::cout;
using std::endl;

class Pillar
{
public:
    double x;
    double y;
    double z1;
    double z2;

    //constructors
    Pillar();
    Pillar(double _x, double _y, double _z1, double _z2);
    Pillar(const Pillar &plr);
    
    //function for sending point to ostream for printing
    void print_all(ostream &)const;

    friend ostream & operator<<(ostream &, const Pillar &);
    friend bool operator < (const Pillar & lhs, const Pillar & rhs);
    friend bool operator > (const Pillar & lhs, const Pillar & rhs);

};

#endif //Pillar_Included