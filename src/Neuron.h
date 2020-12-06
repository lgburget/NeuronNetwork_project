#pragma once

#include "constants.h"
#include "Random.h"

/*! \class Neuron
 * A Neuron is a cell which has four cellular properties (a, b, c, d) and a quality (excitatory or inhibitory) documented in \ref params_.
 * 
 * There are five types of neurons:
 * - inhibitory: FS and LTS
 * - excitatory: RS, IB and RZ
 * 
 * The type of the neuron determines its cellular properties. Values are regrouped in the map \ref Neuron_types.
 * 
 * A neuron has three others attributes which represent its state (membrane potential \ref pot_, membrane recovery variable \ref rec_, synaptic currents \ref curr_). 
 * 
 * A neuron is a piece of a larger system, the neuron \ref Network.
*/
 
/*! \struct Neuron_parameters 
 * A data structure regroups five cellular parameters (a, b, c, d, quality) and their types. Cellular properties don't change during the simulation.
*/
struct Neuron_parameters {double a, b, c, d;
						  bool excit;}; 

class Neuron{
public:
/*! @name Initializing
 * A \ref Neuron is initialized using its type and the noises on a, b, c and d. 
 * Membrane potential and recovery parameters don't depend on the quality of the neuron.
 * \ref Neuron_types is containing all the parameters associated with each type of \ref Neuron.
 */
///@{
	Neuron(const std::string &type, const double &delta);
	static const std::map<std::string, Neuron_parameters> Neuron_types;
///@}
/*! @name Neuron states
 */
///@{
/*!
 * Equation calculates differential equations based on a simple model of spiking neurons.
 * */
	void equation();
	/*
	Potentiel \ref pot_ is updated two times while recovery variable \ref rec_ is only updated once. Therefore there is two separated methods to update them.
	\ref update_rec() also tests if the \ref Neuron is \ref firing(). 
	void update_pot();
	void update_rec();
	void update_if_firing();
	*/
/*!
 * Test if the neuron is firing.
 */
	bool firing() const {return (pot_ > _Discharge_Threshold_);}
///@}

/*! @name Setters/Getters
 */
///@{
	void set_params(const Neuron_parameters& new_params) {params_ = new_params;}
	Neuron_parameters get_params() const {return params_;}
	void set_potential(const double& pot) {pot_ = pot;}
	double get_potential() const {return pot_;}
	void set_recovery(const double& rec) {rec_ = rec;}
	double get_recovery() const {return rec_;}
	void set_current(const double& curr) {curr_ = curr;}
	double get_current() const {return curr_;}
	std::string get_type() const {return n_type;}
///@}
private:
/*! @name Cellular properties
 * Parameters are regrouped in a data structure called \ref Neuron_parameters. They are initialized at the start of the simulation and are constant.
 */
///@{
	Neuron_parameters params_;
	std::string n_type;
///@}
/*! @name State variables
 * State variables change after every step of the simulation.
 */
///@{
	double pot_, rec_, curr_;
///@}
};
