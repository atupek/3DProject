//select_bridge.h
#ifndef Select_Bridge_Included
#define Select_Bridge_Included

double calculate_gain(double height, double length, int num_elements);
double calculate_gain_above_object(int num_elements, double height, double length, double h1, double h2);
double calculate_score(double gain, int num_elements, double lmax);

#endif //Select_Bridge_Included