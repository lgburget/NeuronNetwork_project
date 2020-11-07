#include "Network.h"

Network::Network() 
:
{
}


std::vector<std::pair<size_t, double>> Network::find_neighbours(const size_t &n)
{
	std::vector<std::pair<size_t, double>> neighbours;
	for (std::map<std::pair<size_t, size_t>, double>::iterator it=links.begin(); it!= links.end(); ++it) {
		if (it->first.first == n and it->first.second <= neurons.size()) {
			neighbours.push_back(std::make_pair(link.first.second, link.second));
		}
	}
	
	return neighbours;
}

double Network::total_current(const size_t &n)
{
	double current;
	double noise = RandomNumbers::normal(0,1);
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
