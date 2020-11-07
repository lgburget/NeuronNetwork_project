#pragma once

#include "Neuron.h"
#include "Random.h"
#include <vector>

/*! \class Network
 * A neuron network is a set of \ref Neuron and their connections. 
 * 
 * Neurons are identified by their index in the vector \ref neurons.
 * 
 * Links between \ref neurons are directional links listed in a map \ref links : 
 * the first element of the map is the pair of neurons implicated in the link 
 * (first=receiving neuron, second=sending neuron) and the second element is the 
 * intensity of connection.
 * 

 */
 
 typedef std::map<std::pair<size_t, size_t>, double> Link;
 
class Network {
public:
	//Network();

/*! 
 *Find all neurons connected with incomming connections to neuron \p n.
 *\param n : the index of the receiving neuron.
 *\return a vector of pair {neuron index, link intensity}.
*/
	std::vector<std::pair<size_t, double>> find_neighbours(const size_t &n);
	
/*! 
 *Calculates the total synaptic current received by neuron \p n.
 *\param n : the index of the receiving neuron.
 *\return a double value.
*/
	double total_current(const size_t &n);
	

private:
	std::vector<Neuron> neurons;
	Link links;
	
};

