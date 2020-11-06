#include "constants.h"

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
 */
///@{
	Neuron();
///@}
	void equation();
	bool firing() const {return (pot_ > _Connectivity_);}
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
	Neuron_parameters params_;
	double pot_, rec_, curr_;
};