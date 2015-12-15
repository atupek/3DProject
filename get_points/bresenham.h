//bresenham line algorithm
#ifndef Bresenham_Included
#define Bresenham_Included

#include "process_layers.h"

//modified from java code found at:
// http://tech-algorithm.com/articles/drawing-line-using-bresenham-algorithm/
void bresenham(int x1, int x2, int y1, int y2, pixel_layer &layer);

#endif //Bresenham_Included