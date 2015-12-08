#include <iostream>
using std::cout;
using std::endl;
#include "cube_primitive.h"

int main()
{
	Cube_Primitive prim1(1.2, 1.2, 2.3, 2.3, 1.2, 2.2);
	Cube_Primitive prim2(4.2, 4.2, 5.3, 5.3, 5.2, 7.2);
	if(prim1 < prim2)
	{
		cout << "prim1 < prim2" << endl;
	}
	return 0;
}