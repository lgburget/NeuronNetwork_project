#include "Network.h"

Network::Network()
{}

Network::Network(const size_t& number,const std::string& n_types, const double& d, const double& connectivity, const std::string& model, const double& intensity)
{
	// Fonction that extract types proportions from a given n_types string
	extract_types(n_types, number);

	// Calculation of the number of Neurons of each type
	int nb_RS  = (int)floor(number*types_proportions["RS"]);
	int nb_IB  = (int)floor(number*types_proportions["IB"]);
	int nb_FS  = (int)floor(number*types_proportions["FS"]);
	int nb_LTS = (int)floor(number*types_proportions["LTS"]);
	int nb_CH  = (int)floor(number*types_proportions["CH"]);

	// creation of the good number of each type of neurons
	int start = 0;
	int stop = nb_RS;

	for (int i(start); i<stop; ++i) {
		Neuron n("RS", d);
		neurons.push_back(n);
	}
	start += nb_RS;
	stop += nb_IB;

	for (int i(start); i<stop; ++i) {
		Neuron n("IB", d);
		neurons.push_back(n);
	}
	start += nb_IB;
	stop += nb_FS;

	for (int i(start); i<stop; ++i) {
		Neuron n("FS", d);
		neurons.push_back(n);
	}
	start += nb_FS;
	stop += nb_LTS;

	for (int i(start); i<stop; ++i) {
		Neuron n("LTS", d);
		neurons.push_back(n);
	}
	start += nb_LTS;
	stop += nb_CH;

	for (int i(start); i<stop; ++i) {
		Neuron n("CH", d);
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
		return (int)std::floor(_RNG->poisson(connectivity));
	}

	if(model == "over-dispersed") {
		return (int)std::floor(_RNG->poisson(_RNG->exponential(1.0/connectivity)));
	}
	else return (int)std::floor(connectivity);
}

bool Network::add_link(const size_t& n_r, const size_t& n_s, double i)
{
	if((n_r>=get_size()) or (n_s>=get_size()) or (n_r==n_s)) return false;			// check that the neurons exist and that the two neurons are not actually the same neuron.
	if (not links.count({n_r,n_s}))										// check that the map doesn't already contains a link for these neurons.
	{
		links[{n_r,n_s}] = i;
		return true;
	}else return false;
}

void Network::random_connect(const double& connectivity, const double &i, const std::string &model)
{
		// values that will be picked at random
		int link_number;

		// generation of an index vector containing the index of neurons in the croissant order
		std::vector<size_t> index(get_size());
		for (size_t k(0); k<get_size(); ++k) index[k] = k;

		// For each neuron, a number of connection picked at random is created
		for (size_t j(0); j<get_size(); ++j) {

		_RNG->shuffle(index);                                       // shuffle the vector of index in order to pick neurons at random (corresponding to the order of index)
		link_number = calculate_connections(connectivity, model);

		// Calculation of the number of connections made by the Neuron
		// The first link_number neurons of vector index will be connected to neuron j
		int stop = 0;

		for (size_t m(0); m<get_size() and stop<link_number; ++m) {
			if (add_link(j,index[m],_RNG->uniform_double(0, 2*i))) ++stop;				// intensity of connection is picked at random
		}
	}
}

std::vector<std::pair<size_t, double>> Network::find_neighbours(const size_t &n)
{
	std::vector<std::pair<size_t, double>> neighbours;
	std::pair<size_t, int> key_low(n, 0);								// creating the first possible key corresponding to the neuron n
	std::pair<size_t, int> key_up(n, get_size());						// creating the last possible key corresponding to the neuron n
	Link::iterator itlow = links.lower_bound(key_low);					// create the lower iterator pointing on the first key of the neuron n
	Link::iterator itup = links.upper_bound(key_up);					// creating the upper iterator pointing on the first key of the next neuron (n+1)
	// iterator on the map links to find all links that contains neuron n as a receiving neuron
	for(std::map<std::pair<size_t, size_t>, double>::iterator it(itlow); it != itup and it->first.second < get_size(); ++it) {
			neighbours.push_back(std::make_pair(it->first.second, it->second));
	}
	return neighbours;
}


double Network::valence(const size_t &n)
{
	double valence = 0.0;
	for (const auto& link: find_neighbours(n)) {
		if (neurons[link.first].get_params().excit) valence += link.second;
		else valence -= link.second;
	}
	return valence;
}


bool Network::is_type(const std::string& type) const
{
	for (size_t i(0); i<get_size(); ++i) {
		if (neurons[i].get_type() == type) return true;
	}
	return false;
}

size_t Network::find_first_neuron(const std::string& type) const
{
	for (size_t i(0); i<get_size(); ++i) {
		if (neurons[i].get_type() == type) return i;
	}
	return 0;
}


double Network::total_current(const size_t &n)
{
	double current(0.0);
	double noise = _RNG->normal(0,1);								    // external noise is picked at random
	if (neurons[n].get_params().excit) current = 5.0*noise;
	else current = 2.0*noise;
	for (const auto& neighbour : find_neighbours(n)) {
		if (neurons[neighbour.first].firing()) {					    // check if neighbour is firing and thus sending a signal to neuron n
			if (neurons[neighbour.first].get_params().excit) {
					current+= neighbour.second*0.5;						// if firing and excitatory -> add half of the intensity of current
				}
			else current-=  neighbour.second;							// if firing and inhibitory -> substract the intensity of the current
		}
	}

	return current;
}

std::vector<size_t> Network::update()
{
	std::vector<size_t> firing_neurons(0);					// creation of a temporary vector to store firing neurons and to update them after the others
	for (size_t i(0); i<get_size(); ++i) {
		if(neurons[i].firing()) {
			firing_neurons.push_back(i);					// pushes firing neurons into the temporary vector
		}
		else {
			neurons[i].set_current(total_current(i));
			neurons[i].equation();
		}
	}
	if(not firing_neurons.empty()) {								// the firing neurons are then updated
		for(const auto& n : firing_neurons) neurons[n].reset();
	}
	return firing_neurons;
}

void Network::print_parameters(std::ostream *outstr)
{
    // Print of the header
    *outstr << "Type" << "\t"
		    << "a" << "\t" << "b" << "\t" << "c" << "\t" << "d" << "\t" 
		    << "Inhibitory" << "\t" << "degree" << "\t" << "valence"
		    << std::endl;
    for (size_t i(0); i<get_size(); ++i) {
		  // Print the parameters
		  *outstr << neurons[i].params_to_print()
		  << "\t" << find_neighbours(i).size()
		  << "\t" << valence(i)
		  << std::endl;
      }
}

void Network::print_sample(const int& t, std::ostream *outstr)
{
	  *outstr << t ;
	  for (const auto& type : types_proportions){
		  if(not (type.second == 0.0)) print_properties(type.first, outstr);
	  }
      *outstr << std::endl;
}

void Network::print_properties(const std::string& type, std::ostream *outstr)
{
	size_t n = find_first_neuron(type);
	*outstr << neurons[n].variables_to_print();
}

void Network::header_sample(std::ostream *outstr)
{
	for (const auto& type : types_proportions){
		  if(not (type.second == 0.0)) *outstr << "\t" << type.first<< ".v"
											   << "\t" << type.first << ".u"
											   << "\t" << type.first << ".I";
	  }
      *outstr << std::endl;
}
