#include "neural_net.h"

Neuron::Neuron()
{
	num_inputs = 0;
	weights_out = {}; //empty vector
	output = 0.0;
}


Neuron::Neuron(int _num_inputs, vector<double> _weights_out)
{
	num_inputs = _num_inputs;
	weights_out = _weights_out;
}

double Neuron::calculate_output()
{
	//sum weights coming in
	return 0.0;
}

/*
Neural_Net::Neural_Net()
{

}*/

