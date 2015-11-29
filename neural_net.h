//process bitmap
#ifndef Neural_Network_Included
#define Neural_Network_Included

#include<vector>
using std::vector;

class Neuron
{
public:
	int num_inputs;
	vector<double> weights;
	double output;
	Neuron();
};

typedef vector<Neuron> neuron_layer;

class Neural_Net
{
public:
	vector<neuron_layer> network;
	Neural_Net();

};

#endif //Neural_Network_Included