#pragma once

#include "Neuron.h"
#include "Random.h"

/*! \class Network
 * A neuron network is a set of \ref Neuron and their connections.
 * Each \ref Neuron send signal to a unique one, but can receive signal from several ones.
 *
 * Neurons are identified by their index in the vector \ref neurons.
 *
 * Links between \ref neurons are directional links listed in a map \ref links : the first
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
	Network(int number, double prop_excitatory, double connectivity, double intensity);
///@}

/*!
  Provide access to the set of \ref Neuron.
*/
	std::vector<Neuron> get_neurons() const { return neurons ; };
	
	
/*!
  Allows the test program to modify the potential of a neuron in the network
 */
	void set_neuron_potential(const size_t &n, const double& pot) { neurons[n].set_potential(pot); }
/*!
  Provides access to the set of \ref links.
 */
	Link get_links() const { return links ; };
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
    void random_connect(const double& lambda, const double &i);
/*!
 *Find all neurons connected with incomming connections to neuron \p n.
 *\param n : the index of the receiving neuron.
 *\return a vector of pair {neuron index, link intensity}.
*/
	std::vector<std::pair<size_t, double>> find_neighbours(const size_t &n);
	
/*!
 *Tests if the neuron in parameter is firing
*/
	bool neuron_firing (const Neuron &neuron_) const;
	
/*!
 *Tests if the \ref Neuron \p n is already sending signal to another one (a neuron can only send to a unique other neuron)
*/
	bool is_sending(const size_t& n) const;

/*!
 *Calculates the total synaptic current received by neuron \p n.
 *\param n : the index of the receiving neuron.
 *\return a double value.
*/
	double total_current(const size_t &n);

/*!
  In order to perform one time-step of the simulation, it updates twice the potential and once the recovery. 
*/
	void update();



private:
	std::vector<Neuron> neurons;
	Link links;

};
