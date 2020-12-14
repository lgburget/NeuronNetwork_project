#pragma once

#include "Neuron.h"

/*! \class Network
 * A neuron network is a set of \ref Neuron and their connections.
 * Each \ref Neuron sends and receives signal from several other ones, thus creating a network.
 *
 * Neurons are identified by their index in the vector \ref neurons.
 *
 * Links between \ref neurons are directional links listed in the map \ref links : the first
 * element of the map is the pair of neurons implicated in the link (first=receiving neuron,
 * second=sending neuron) and the second element is the intensity of connection.
 */

typedef std::map<std::pair<size_t, size_t>, double> Link;

class Network {
public:


/*! @name Initializing
 */
///@{
	Network();
/*!
 * The constructor creates a defined \p number of neurons in the \ref Network and links them using \ref random_connect.
 * The connections are defined by their distribution ( \p model), their average number per \ref Neuron ( \p connectivity) and their \p intensity. Each of these parameters are either chosen by the user or initialized by default
 * The noises applied on the parameters of each \ref Neuron in the \ref Network are initialized with the user's parameter \p d.
 * The proportions of each type of \ref Neuron are passed as parameter via the string \p n_types and are read using \ref extract_types
 * \param number: number: total number of \ref Neuron in the \ref Network
 * \param n_types: string to be read, containing neuron type proportions
 * \param d: defines the length of the interval for neuron parameters to be picked at random in
 * \param connectivity: average number of connection for a neuron
 * \param model: dispersion model to pick number of connection at random
 * \param intensity: average intensity of connections
 */
	Network(const size_t& number,const std::string& n_types, const double& d, const double& connectivity, const std::string& model, const double& intensity);

/*!
 * Allows to extract from a string the proportion of each specific type of \ref Neuron
 * \param n_types: string to be read 
 * \param number: total number of \ref Neuron in the \ref Network
 */
	void extract_types(std::string n_types, int number);
///@}

/*! @name Getters/setters
 */
///@{
/*!
 * Provide access to the set of \ref Neuron.
 */
	std::vector<Neuron> get_neurons() const { return neurons ; }
	
/*!
 * Provides access to the set of \ref links.
 */
	Link get_links() const { return links ; }

/*!
 * Provides access to the \ref noises
 */
	std::vector<double> get_noises() const { return noises; }
/*!
 * Provides access to the potential of neuron \p n
 */
	double get_potential(const size_t& n) const { return neurons[n].get_potential(); }
/*!
 * Provides access to the recovery of neuron \p n
 */
	double get_recovery(const size_t& n) const { return neurons[n].get_recovery(); }
/*!
 * Provides access to the current of neuron \p n
 */
	double get_current(const size_t& n) const { return neurons[n].get_current(); }
	size_t get_size() const { return neurons.size(); }
/*!
 * Allows the test program to modify the potential of a \ref Neuron in the network
 * \param n (size_t): neuron to change potential
 * \param pot (double): new potential value
 */
	void set_neuron_potential(const size_t &n, const double& pot) { neurons[n].set_potential(pot); }
 ///@}
 
/*! @name Linking neurons
 */
///@{
/*!
 * Calculates the number of connections that the \ref Neuron will make
 * \param connectivity (double): mean connectivity of the chosen model
 * \param model (std::string): chosen model
 */	
	int calculate_connections(double connectivity, std::string model);
/*!
 * Creates a new link in the map \ref links.
 * \param n_r (size_t): receiving neuron,
 * \param n_s (size_t): sending neuron,
 * \param i (double): link intensity.
 */
    bool add_link(const size_t& n_r, const size_t& n_s, double i);
/*!
 * Creates all the random links of the network.
 * Each \ref Neuron will expect to receive the inputs of n other neurons, calculated with \ref calculate_connections.
 * The intensity of the link is picked at random using uniform distribution between 0 and 2* \p i.
 * The sending neurons will be picked at random within \ref neurons, but as a \ref Neuron can only send
 * signal to a unique one, the expected number of connection may not be reached.
 * \param connectivity (double): mean value of connectivity.
 * \param i (double): mean value of the uniform distribution (with bounds 0 and 2*i)
 * \param model (std::string): model to pick number of links at random.
 */
    void random_connect(const double& connectivity, const double &i, const std::string &model);
/*!
 *Find all neurons connected with incomming connections to neuron \p n.
 *\param n : the index of the receiving neuron.
 *\return a vector of pair {neuron index, link intensity}.
*/
	std::vector<std::pair<size_t, double>> find_neighbours(const size_t &n);
/*!
 * Calculate the sum of intensity of all neurons connected to neuron \p n.
 *\param n : the index of the receiving neuron.
 *\return the valence of neuron \p n.
*/
	double valence(const size_t &n);
	
///@}
	
	
/*! @name Testing neurons properties
 */
///@{
/*!
 *Tests if the neuron in parameter is firing
 */
	bool neuron_firing (const Neuron &neuron_) const {return neuron_.firing();}
/*!
 *Tests if there is at least one \ref Neuron of type \p type in the network
 */
	bool is_type(const std::string& type) const;
/*!
 * finds the index of the first \ref Neuron of one type of neuron passed as a parameter
 */
	size_t find_first_neuron(const std::string& type) const;
///@} 

/*! @name Running the simulation
 */
///@{
/*!
 *Calculates the total synaptic current received by neuron \p n.
 *\param n : the index of the receiving neuron.
 *\return a double value.
*/
	double total_current(const size_t &n);

/*!
 * Principal function that updates the parameters of each \ref Neuron in the \ref Network
 * It returns a vector containing the index of firing neurons to allow the \ref Simulation to have access to them.
 * In order to perform one time-step of the simulation, it updates twice the potential and once the recovery. 
 */
	std::vector<size_t> update();
///@}

/*! @name Generating the output
 * \ref print_sample prints the properties of some neurons at each simulation steps, and \ref print_parameters prints parameters of each \ref Neuron.
 */
///@{
/*!
 * Print the parameters of every neurons in the \ref Network
 */
	void print_parameters(std::ostream *outstr);	
/*!
 * Print the potential, recovery and current of the first \ref Neuron in the \ref Network at each simulation step using \ref print_properties
 * \param t is the simulation time step
 * \param *outstr is the output stream for writing values
 */					
	void print_sample(const int& t, std::ostream *outstr);
/*!
 * Helper function for \ref print_sample
 */
	void print_properties(const std::string& type, std::ostream *outstr);
/*!
 * Print a header for function \ref print_sample
 */
	void header_sample(std::ostream *outstr);							
///@}
private:
/*!
 * Set of \ref Neuron that composes the network. 
 */
	std::vector<Neuron> neurons;

/*!
 * Set of proportions of each specific type of \ref Neuron
 * This set is initialized with the function \ref extract_types
 */
	std::map<std::string, double> types_proportions {
	{"RS",  0.0}, 
	{"IB",  0.0},
	{"FS",  0.0},
	{"LTS", 0.0},
	{"CH",  0.0}
	};
/*!
 * List of the parameters' noises
 */
	std::vector<double> noises;

/*!
 * List of directional links between \ref Neuron. 
 */
	Link links;

};
