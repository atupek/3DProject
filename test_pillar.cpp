#include "pillar.h"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	Pillar new_pillar(0.0, 0.0, 0.0, 1.0);
	Pillar new_pillar1(1.0, 0.0, 1.0, 2.0);
	if(new_pillar < new_pillar1)
	{
		cout << "new_pillar is less than new_pillar1" << endl;
	}
	new_pillar.print_all(cout);
	new_pillar1.print_all(cout);
	return 0;
}