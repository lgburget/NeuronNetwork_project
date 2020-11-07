#include "Network.h"
#include <cmath>

Network::Network() 
{}

Network::Network(int number, double prop_excitatory, double connectivity, double intensity)
{
	int nb_excitatory = (int)floor(number*prop_excitatory);
	for (size_t i(0); i<nb_excitatory; ++i) {
		Neuron n(true);
		neurons.push_back(n);
	}
	for (size_t i(nb_excitatory); i<number; ++i) {
		Neuron n(false);
		neurons.push_back(n);
	}
	
	// il faudra que j'ajoute la création des liens grace a une fonction add_link: 
	// nombre de lien par neurone (Pois(connectivity)), 
	// choix aléatoire des neurones
	// intensité du lien
}


std::vector<std::pair<size_t, double>> Network::find_neighbours(const size_t &n)
{
	std::vector<std::pair<size_t, double>> neighbours;
	for (std::map<std::pair<size_t, size_t>, double>::iterator it=links.begin(); it!= links.end(); ++it) {
		if (it->first.first == n and it->first.second <= neurons.size()) {
			neighbours.push_back(std::make_pair(it->first.second, it->second));
		}
	}
	
	return neighbours;
}

double Network::total_current(const size_t &n)
{
	double current;
	double noise = _RNG->normal(0,1);
	if (neurons[n].get_params().excit) current = 5*noise;
	else current = 2*noise;
	
	for (auto neighbour: find_neighbours(n)) {
		if (neurons[neighbour.first].firing()) {
			if (neurons[neighbour.first].get_params().excit) current+=neighbour.second /2;
			else current-=neighbour.second /2;
		}
	}
	
	return current;
}

void Network::update()
{
	for (size_t i(0); i<neurons.size(); ++i) {
		neurons[i].set_current(this->total_current(i));
		neurons[i].equation();
	}
}
