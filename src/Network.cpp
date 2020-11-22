#include "Network.h"
#include <cmath>

Network::Network()
{}

Network::Network(int number, double prop_excitatory, double connectivity, double intensity)
{
	int nb_excitatory = (int)floor(number*prop_excitatory);
	// creation of nb_excitatory excitatory neurons
	for (int i(0); i<nb_excitatory; ++i) {
		Neuron n(true);
		neurons.push_back(n);
	}
	// The rest of neurons are inhibitory
	for (int i(nb_excitatory); i<number; ++i) {
		Neuron n(false);
		neurons.push_back(n);
	}
	// Creation of all links between neurons
	random_connect(connectivity, intensity);
}

void Network::add_link(const size_t& n_r, const size_t& n_s, double i)
{
	if(n_r == n_s) return;                                             // check that the two neurons are not actually the same neuron
	if (not links.count({n_r,n_s})) links[{n_r,n_s}] = i;			   // check that the map doesn't already contains a link for these neurons
}

void Network::random_connect(const double& lambda, const double &i)
{
	// values that will be picked at random
	int link_number;
	double intensity;
	
	// generation of an index vector containing the index of neurons in the croissant order
	std::vector<size_t> index(neurons.size());
	for (size_t k(0); k<neurons.size(); ++k) index[k] = k;
	
	// For each neuron, a number of connection picked at random is created
	for (size_t j(0); j<neurons.size(); ++j) {
		_RNG->shuffle(index);                                                   // shuffle the vector of index in order to pick neurons at random (corresponding to the order of index)
		link_number = _RNG->poisson(lambda);                                    // Random number of link for the neuron j
		// The first link_number neurons of vector index will be connected to neuron j
		size_t stop = link_number; 
		for (size_t m(0); m<stop; ++m) {
			intensity = _RNG->uniform_double(0, 2*i);                           // intensity of connection is picked at random
			if (not this->is_sending(index[m])) add_link(j,index[m],intensity); // check that neuron[m] doesn't already send a signal to an other neuron to add link
			else ++stop; 			                    // if the neuron[m] already send a signal to an other neuron, we will choose the next neuron so we shift the stop value
		}
	}
}


std::vector<std::pair<size_t, double>> Network::find_neighbours(const size_t &n)
{
	std::vector<std::pair<size_t, double>> neighbours;
	// iterator on the map links to find all links that contains neuron n as a receiving neuron
	for (std::map<std::pair<size_t, size_t>, double>::iterator it=links.begin(); it!= links.end(); ++it) {
		if (it->first.first == n and it->first.second <= neurons.size()) {
			neighbours.push_back(std::make_pair(it->first.second, it->second));
		}
	}

	return neighbours;
}


bool Network::neuron_firing (const Neuron &neuron_) const
{
		if (neuron_.firing()) return true;
		else return false;
}

bool Network::is_sending(const size_t& n) const
{
	for (size_t i(0); i<neurons.size(); ++i) {
		if (links.count({i,n}) == 1) return true;
	}
	return false;
}


double Network::total_current(const size_t &n)
{
	double current;
	double noise = _RNG->normal(0,1);								// external noise is picked at random
	if (neurons[n].get_params().excit) current = 5.0*noise;										
	else current = 2.0*noise;

	for (auto neighbour: find_neighbours(n)) {
		if (neurons[neighbour.first].firing()) {						// check if neighbour is firing and thus sending a signal to neuron n										
			if (neurons[neighbour.first].get_params().excit) {
				current+= (double)links[{n, neighbour.first}]*0.5;		// if firing and excitatory -> add half of the intensity of current
				}		
			else current-=links[{neighbour.second, neighbour.first}];	// if firing and inhibitory -> substract the intensity of the current
		}
	}

	return current;
}




void Network::update()
{
	// First loop to update just the potential 
	for (size_t i(0); i<neurons.size(); ++i) {
			neurons[i].set_current(this->total_current(i));
			neurons[i].update_pot();
	}
	// Second loop to update both potential and recovery
	for (size_t i(0); i<neurons.size(); ++i) {
			neurons[i].set_current(this->total_current(i));
			neurons[i].update_pot();
			neurons[i].update_rec();
	}
}
