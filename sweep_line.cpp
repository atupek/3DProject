//sweep_plane class source
#include "sweep_line.h"

Sweep_line::Sweep_line()
{
	intersected_points = {};
}

//print function to print point coordinates
void Sweep_line::print_sweep_line_members(ostream & os) const
{
	cout << "printing sweep line members: " << endl;
	cout << "\tnumber of intersected_points: " << intersected_points.size() << endl;
	for(auto i = intersected_points.begin(); i != intersected_points.end(); i++)
	{
		os << "\tPoint: ";
		i->first.print_coords(cout);
		os << "\tSegment: ";
		i->second.print_coords(cout);
	}
}