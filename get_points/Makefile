CC=g++
CFLAGS= -I. -c
CXXFLAGS= -std=c++11
DEPS = bresenham.h point.h process_bitmap.h process_gcode.h process_layers.h process_pixels.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

get_points: getPoints.o point.o bresenham.o process_bitmap.o process_gcode.o process_layers.o process_pixels.o
	g++ -o get_points getPoints.o point.o bresenham.o process_bitmap.o process_gcode.o process_layers.o process_pixels.o -std=c++11 -I. -c

clean:
	rm -f *.o