#include "Network.h"
#include <cmath>

Network::Network()
{}

Network::Network(int number, double prop_excitatory, double connectivity, double intensity)
{
	int nb_excitatory = (int)floor(number*prop_excitatory);
	for (int i(0); i<nb_excitatory; ++i) {
		Neuron n(true);
		neurons.push_back(n);
	}
	for (int i(nb_excitatory); i<number; ++i) {
		Neuron n(false);
		neurons.push_back(n);
	}

	random_connect(connectivity, intensity);
}

void Network::add_link(const size_t& n_r, const size_t& n_s, double i)
{
	if (not links.count({n_r,n_s})) links[{n_r,n_s}] = i;
}

void Network::random_connect(const double& lambda, const double &i)
{
	int link_number;
	double intensity;
	std::vector<size_t> index;
	for (size_t k(0); k<neurons.size(); ++k) index[k] = k;
	for (size_t j(0); j<neurons.size(); ++j) {
		_RNG->shuffle(index);
		link_number = _RNG->poisson(lambda);
		for (int m(0); m<link_number; ++m) {
			intensity = _RNG->uniform_double(0, 2*i);
			if (not this->is_sending(index[m])) add_link(j,index[m],intensity);
		}
	}
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


bool Network::neuron_firing (const Neuron &neuron_) const
{
			if (neuron_.firing()) {return true;}
		else  {	return false ;}
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
	double noise = _RNG->normal(0,1);
	if (neurons[n].get_params().excit) current = 5*noise;
	else current = 2*noise;

	for (auto neighbour: find_neighbours(n)) {
		if (neurons[neighbour.first].firing()) {
			if (neurons[neighbour.first].get_params().excit) current+=neighbour.second /2;
			else current-=neighbour.second;
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
