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
 * A \ref Neuron is initialized using its type and the noise on its parameters a, b, c and d. 
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
 */
	void equation();
/*!
 * Tests if the neuron is firing.
 */
	bool firing() const {return (pot_ > _Discharge_Threshold_);}
///@}

/*! @name Setters/Getters
 */
///@{

/*!
 * Allows other classes to set the parameters \ref params_ of the \ref Neuron using \p new_params
 */
	void set_params(const Neuron_parameters& new_params) {params_ = new_params;}
/*!
 * Provides access the parameters \ref params_
 */
	Neuron_parameters get_params() const {return params_;}
/*!
 * Allows other classes to set the potential \ref pot_ of the \ref Neuron using \p pot
 */
	void set_potential(const double& pot) {pot_ = pot;}
/*!
 * Provides access the potential \ref pot_
 */
	double get_potential() const {return pot_;}
/*!
 * Allows other classes to set the recovery \ref rec_ of the \ref Neuron using \p rec
 */
	void set_recovery(const double& rec) {rec_ = rec;}
/*!
 * Provides access the recovery \ref rec_
 */
	double get_recovery() const {return rec_;}
/*!
 * Allows other classes to set the current \ref curr_ of the \ref Neuron using \p curr
 */
	void set_current(const double& curr) {curr_ = curr;}
/*!
 * Provides access the current \ref curr_
 */
	double get_current() const {return curr_;}
/*!
 * Provides access the name of the type \ref n_type of the \ref Neuron
 */
	std::string get_type() const {return n_type;}
	
///@}

/*! @name Helpers
 */
///@{
/*!
 * Returns a string containing the parameters of the \ref Neuron
 */	
	std::string params_to_print() const;	
/*!
 * Returns a string containing the recovery \ref rec_, potential \ref pot_ and current \ref curr_ of the \ref Neuron
 */								
	std::string variables_to_print() const;	
/*!
 * Is used to reset the potential \ref pot_ and the recovery \ref rec_ . 
 * This function is called by the update in the \ref Network
 */							
	void reset() {pot_ = params_.c; rec_ += params_.d;}					
///@}

private:
/*! @name Cellular properties
 * Parameters are regrouped in a data structure called \ref Neuron_parameters. They are initialized at the start of the simulation and are constant.
 * \ref n_type holds the name of the type of the \ref Neuron
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
