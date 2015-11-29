//process bitmap
#ifndef Neural_Network_Included
#define Neural_Network_Included

#include<vector>
using std::vector;

class Neuron
{
public:
	int num_inputs;
	vector<double> weights_out;
	double output;
	Neuron();
	Neuron(int num_inputs, vector<double> weights_out);
	double calculate_output();
};

typedef vector<Neuron> neuron_layer;

//don't know if I actually need a neural_net class or if I can just go with the layers of neurons...
/*
class Neural_Net
{
public:
	vector<neuron_layer> network;
	Neural_Net();

};*/

#endif //Neural_Network_Included