//bresenham line algorithm
#include "bresenham.h"

//modified from java code found at:
// http://tech-algorithm.com/articles/drawing-line-using-bresenham-algorithm/
void bresenham(int x1, int x2, int y1, int y2, pixel_layer &layer)
{
	int w = x2 - x1;
	int h = y2 - y1;
	int dx1 = 0;
	int dx2 = 0;
	int dy1 = 0;
	int dy2 = 0;

	//what about if w == 0 or h == 0???
	//looks like it still works...
	if (w < 0)
	{
		dx1 = -1;
		dx2 = -1;
	}
	else
	{
		dx1 = 1;
		dx2 = 1;
	}
	if (h < 0)
	{
		dy1 = -1;
	}
	else
	{
		dy1 = 1;
	}

	int longest = abs(w);
	int shortest = abs(h);

	if(!(longest > shortest))
	{
		longest = abs(h);
		shortest = abs(w);
		if(h < 0)
		{
			dy2 = -1;
		}
		else
		{
			dy2 = 1;
		}
		dx2 = 0;
	}

	int numerator = longest >> 1;
	for(int i = 0; i <= longest; i++)
	{
		//set x, y = 1.0;
		layer[x1][y1] = 1.0;
		numerator += shortest;
		if(! (numerator < longest))
		{
			numerator -= longest;
			x1 += dx1;
			y1 += dy1;
		}
		else
		{
			x1 += dx2;
			y1 += dy2;
		}
	}
}