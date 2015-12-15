//neural_net class tester
#include "Neural_net.h"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
	int input_size = 8;
	int hidden_size = 4;
	int out_layer_size = 2;
	neuron_layer input_layer;
	neuron_layer hidden_layer;
	neuron_layer output_layer;

	for(auto i = 0; i < input_size; i++)
	{
		Neuron new_neuron;
		input_layer.push_back(new_neuron);
	}

	for(auto i = 0; i < hidden_size; i++)
	{
		Neuron new_neuron(8);
		hidden_layer.push_back(new_neuron);
	}

	for(auto i = 0; i < out_layer_size; i++)
	{
		Neuron new_neuron(4);
		output_layer.push_back(new_neuron);
	}

	cout << "input size: " << input_layer.size() << endl;
	cout << "hidden size: " << hidden_layer.size() << endl;
	cout << "output size: " << output_layer.size() << endl;

	return 0;
}