#pragma once

#include "constants.h"
#include "Random.h"

/*! \class Neuron
 * The Neuron class creates a neuron which has four cellular properties (a, b, c, d), a quality (excitatory or inhibitory)
 * and three others attributes which represent its state (membrane potential, membrane recovery variable, synaptic currents). 
 * A neuron is a piece of a larger system called \ref Network.
*/
 
/*! \struct Neuron_parameters 
 * A data structure regroups five cellular parameters (a, b, c, d, quality) and their types. Cellular properties don't change during the simulation.
*/
struct Neuron_parameters {double a, b, c, d;
						  bool excit;}; 

class Neuron{
public:
/*! @name Initializing
 * A neuron is initialized using its quality (excitatory or inhibitory), parameters are a combination of constants and uniform distributions. Membrane potential and recovery parameters don't depend on the quality of the neuron.
 */
///@{
	Neuron(bool excit);
///@}
/*! @name Neuron states
 * Potentiel \ref pot_ is updated two times while recovery variable \ref rec_ is only updated once. Therefore there is two separated methods to update them.
 * \ref update_rec() also tests if the \ref Neuron is \ref firing(). 
 */
///@{
	// Equation calculates differential equations based on a simple model of spiking neurons.
	//void equation();
	void update_pot();
	void update_rec();
	void update_if_firing();
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
///@}
private:
/*! @name Cellular properties
 * Parameters are regrouped in a data structure called \ref Neuron_parameters. They are initialized at the start of the simulation and are constant.
 */
///@{
	Neuron_parameters params_;
///@}
/*! @name State variables
 * State variables change after every step of the simulation.
 */
///@{
	double pot_, rec_, curr_;
///@}
};
