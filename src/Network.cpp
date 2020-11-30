#include "Network.h"

Network::Network()
{}

Network::Network(size_t number, std::string n_types, double d, double connectivity, std::string model, double intensity)
{
	// Initialisation of the noises
	for(int i(0); i<4; ++i) {
		double x_noise = _RNG->uniform_double(1.0 - d, 1.0 + d);
		noises.push_back(x_noise);
	}

	// Fonction that extract types proportions from a given n_types string
	extract_types(n_types, number);

	// Calculation of the number of Neurons of each type
	int nb_RS  = (int)floor(number*types_proportions["RS"]);
	int nb_IB  = (int)floor(number*types_proportions["IB"]);
	int nb_FS  = (int)floor(number*types_proportions["FS"]);
	int nb_LTS = (int)floor(number*types_proportions["LTS"]);
	int nb_RZ  = (int)floor(number*types_proportions["RZ"]);

	// creation of the good number of each type of neurons
	int start = 0;
	int stop = nb_RS;

	for (int i(start); i<stop; ++i) {
		Neuron n("RS", noises);
		neurons.push_back(n);
	}
	start += nb_RS;
	stop += nb_IB;

	for (int i(start); i<stop; ++i) {
		Neuron n("IB", noises);
		neurons.push_back(n);
	}
	start += nb_IB;
	stop += nb_FS;

	for (int i(start); i<stop; ++i) {
		Neuron n("FS", noises);
		neurons.push_back(n);
	}
	start += nb_FS;
	stop += nb_LTS;

	for (int i(start); i<stop; ++i) {
		Neuron n("LTS", noises);
		neurons.push_back(n);
	}
	start += nb_LTS;
	stop += nb_RZ;

	for (int i(start); i<stop; ++i) {
		Neuron n("RZ", noises);
		neurons.push_back(n);
	}

	// Creation of all links between neurons
	random_connect(connectivity, intensity, model);
}

void Network::extract_types(std::string n_types, int number)
{
	int size = number;

	if(n_types.empty()) {
		types_proportions["RS"] = 0.5;
		types_proportions["FS"] = 0.5;
	}

	else {
		n_types.erase(std::remove_if(n_types.begin(), n_types.end(), isspace), n_types.end());
        std::string label;
        std::stringstream ss(n_types);
        double total = 0.0;

        for (std::string item; total<size && std::getline(ss, item, ','); ) {
            size_t split = item.find(':');
            label = item.substr(0, split);
            double prop = std::min(std::stod(item.substr(split+1)), 1.0);

            if(prop>0 && label!="RS" && types_proportions.count(label)) {
				if(total + prop > 1.0) prop = 1.0 - total;

				types_proportions[label] = prop;
				total += prop;
			}
		}
		if(total <= 1.0) types_proportions["RS"] = 1.0 - total;
	}
}

int Network::calculate_connections(double connectivity, std::string model)
{
	if(model == "constant") return (int)std::floor(connectivity);

	if(model == "poisson") {
		int c = _RNG->poisson(connectivity);
		return (int)std::floor(c);
	}

	if(model == "over-dispersed") {
		double l = _RNG->exponential(1.0/connectivity);
		int c = _RNG->poisson(l);
		return (int)std::floor(c);
	}
	else return (int)std::floor(connectivity);
}

void Network::add_link(const size_t& n_r, const size_t& n_s, double i)
{
	if(n_r==n_s) return;                                            // check that the two neurons are not actually the same neuron
	if((n_r or n_s)>neurons.size()) return;							// check that the neurons exist
	if (not links.count({n_r,n_s})) links[{n_r,n_s}] = i;			// check that the map doesn't already contains a link for these neurons
}

void Network::random_connect(const double& connectivity, const double &i, std::string &model)
{
		// values that will be picked at random
		int link_number;
		double intensity;

		// generation of an index vector containing the index of neurons in the croissant order
		std::vector<size_t> index(neurons.size());
		for (unsigned long k(0); k<=neurons.size(); ++k) index[k] = k;

		// For each neuron, a number of connection picked at random is created
		for (unsigned long j(0); j<=neurons.size(); ++j) {

		_RNG->shuffle(index);                                       // shuffle the vector of index in order to pick neurons at random (corresponding to the order of index)
		link_number = calculate_connections(connectivity, model);

		// Calculation of the number of connections made by the Neuron
		// The first link_number neurons of vector index will be connected to neuron j
		unsigned long stop = link_number;

//PROBLEME : Le programme crash a ce niveau !
		for (unsigned long m(0); m<stop; ++m) {

			intensity = _RNG->uniform_double(0, 2*i); 	 		// intensity of connection is picked at random

//PROBLEME!! : NE RENTRE JAMAIS DANS LE IF, TOUJOURS DANS LE ELSE!
			if ((not (this->is_sending(index[m]))) ) {					// check that neuron[m] doesn't already send a signal to an other neuron to add link

				add_link(j,index[m],intensity); }
			else  { ++stop;	 } 							// if the neuron[m] already send a signal to an other neuron, we will choose the next neuron so we shift the stop value
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
		return neuron_.firing();
}


// PROBLEME : Elle rend toujours TRUE !
bool Network::is_sending(const size_t& n) const
{
	for (unsigned long i(0); i<neurons.size(); ++i) {
		if (links.count({i,n}) == 1){ return true; }
	}
		return false;
}


double Network::total_current(const size_t &n)
{
	double current(0.0);
	double noise = _RNG->normal(0,1);								    // external noise is picked at random
	if (neurons[n].get_params().excit) current = 5.0*noise;
	else current = 2.0*noise;

	for (auto neighbour: find_neighbours(n)) {
		if (neurons[neighbour.first].firing()) {					    // check if neighbour is firing and thus sending a signal to neuron n
			if (neurons[neighbour.first].get_params().excit) {
					current+= (double)links[{n, neighbour.first}]*0.5;	// if firing and excitatory -> add half of the intensity of current
				}
			else current-=links[{n, neighbour.first}];					// if firing and inhibitory -> substract the intensity of the current
		}
	}

	return current;
}




void Network::update()
{
	for (unsigned long i(0); i<neurons.size(); ++i) {
		if (neurons[i].firing()) {									   // In case neuron is firing parameters are reset
			neurons[i].update_if_firing();
		} else {													   // otherwise, potential is updated twice and recovery once
			// first loop, only potential is updated
			neurons[i].set_current(this->total_current(i));
			neurons[i].update_pot();
			// second loop, both potential and recovery are updated
			//neurons[i].set_current(this->total_current(i));
			neurons[i].update_pot();
			neurons[i].update_rec();
		}
	}


	/*// Second loop to update both potential and recovery
	for (size_t i(0); i<neurons.size(); ++i) {
		neurons[i].set_current(this->total_current(i));
		neurons[i].update_pot();
		neurons[i].update_rec();
	}
	*/
}
