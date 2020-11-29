#pragma once

#include "Neuron.h"

/*! \class Network
 * A neuron network is a set of \ref Neuron and their connections.
 * Each \ref Neuron send a signal to a unique one, but can receive signal from several ones.
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
 * The constructor creates all the neurons of the network and links them randomly using \ref random_connect.
 * The noises applied on the parameters of each \ref Neuron in the \ref Network are initialized with the user's parameter d 
 */
	Network(size_t number, std::string n_types, double d, double connectivity, std::string model, double intensity);

/*!
 * Allows to extract from a string the proportion of each specific type of \ref Neuron
 * \param n_types: string to be read 
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
    void add_link(const size_t& n_r, const size_t& n_s, double i);
/*!
 * Creates all the random links of the network.
 * Each \ref Neuron will expect to receive the inputs of n other neurons, randomly choosed using
 * a poisson distribution with mean \p lambda.
 * The intensity of the link is choosed using uniform distribution between 0 and 2* \p i.
 * The sending neurons will be picked at random within \ref neurons, but as a \ref Neuron can only send
 * signal to a unique one, the expected number of connection may not be reached.
 * \param lambda (double): mean value of Poisson distribution.
 * \param i (double): mean value of the uniform distribution (with bounds 0 and 2*i).
 */
    void random_connect(const double& connectivity, const double &i, std::string &model);
/*!
 *Find all neurons connected with incomming connections to neuron \p n.
 *\param n : the index of the receiving neuron.
 *\return a vector of pair {neuron index, link intensity}.
*/
	std::vector<std::pair<size_t, double>> find_neighbours(const size_t &n);
	
///@}
	
	
/*! @name Testing neurons properties
 */
///@{
/*!
 *Tests if the neuron in parameter is firing
 */
	bool neuron_firing (const Neuron &neuron_) const;
	
/*!
 *Tests if the \ref Neuron \p n is already sending signal to another one (a neuron can only send to a unique other neuron)
 */
	bool is_sending(const size_t& n) const;
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
 * In order to perform one time-step of the simulation, it updates twice the potential and once the recovery. 
 */
	void update();
///@}


private:
/*!
 * Set of \ref Neuron that composes the network. 
 */
	std::vector<Neuron> neurons;

/*!
 * Set of proportions of each specific type of \ref Neuron
 */
	std::map<std::string, double> types_proportions {
	{"RS",  0.0}, 
	{"IB",  0.0},
	{"FS",  0.0},
	{"LTS", 0.0},
	{"RZ",  0.0}
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
