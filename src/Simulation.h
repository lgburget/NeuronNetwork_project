#pragma once

#include "Network.h"

/*!
 * The \b Simulation class is the main class in this program. It constructs the neuron \ref Network according to user-specified parameters, and \ref run the simulation.
 *
 * Simulation results are printed in the file \ref outfile : \ref run prints, for every neuron, for every step of the simulation a 1 if the neuron is in firing state and 0 if not.
 * Moreover, two other files are generated with values of neurons properties.
 *
 * Time advances from 0 until it reaches \ref endtime.
 *
 * A Simulation is made of a \ref Network composed of a given \ref number of \ref Neuron.
 *
 * The parameters of the Simulation are:
 * - \ref endtime : total number of time-steps,
 * - \ref number : total number of neurons,
 * - \ref connectivity : average connectivity of a \ref Neuron,
 * - \ref intensity : average intensity of connections,
 * - \ref model : choosen model for random picking
 * - \ref n_types : proportions of each type of \ref Neuron
 * - \ref d : margin applied to the the parameters of each \ref Neuron 
*/

class Simulation {
public:
/*! @name Initialization and destruction of the simulation
 * The constructor takes the command-line options and extracts all the private attributes from them to create the simulation.
*/
///@{
/*!
 * First the command-line options are extracted.
 *
 * Their value are then verified and if there is no abberant value, attributes are initialized with them.
 *
 * Finally, the neuron \ref Network of the similation is created using the simulation parameters.
 */
		Simulation(int, char **);

/*!
 * The destructor delete the neuron network created for the simulation.
 */
		~Simulation();
///@}


/*! @name Running the simulation
 * The simulation is performed by calling \ref run, which updates the neuron \ref Network , calls for the print methods of the \ref Network and prints 
 * the state of each \ref Neuron each simulation step.
 */
///@{
/*!
 * \ref run is the function that performs the simulation. 
 * It creates all the files that will be written on and calls for the printing fuctions of the \ref Network
 * It iterates on the simulation time and for each step, first updates the neuron \ref Network, on the \ref outfile the state of each \ref Neuron,
 * a 1 if it's firing and a 0 if it's not.
 * Finally, this method closes the files when the \ref endtime has been attained 
 */
		void run();
///@}

private:
/*!
 * The neuron \ref Network of the simulation
 */
		Network* network;
/*!
 * Total time of the simulation
 */
  		int endtime;
/*!
 * Total number of \ref Neuron in the \ref Network
 */
  		size_t number;
/*!
 * Proportion of each specific type of \ref Neuron
 */
		std::string n_types;
/*!
 * Margin applied on the parameters of specific \ref Neuron types
 */
		double d;
/*!
 * Defines the names of the different models for the connectivity \ref model
 */
		std::vector<std::string> allowed;
/*!
 * Model holds the name of model chosen by the user or the name of the defalut model
 */
		std::string model;
/*!
 * Average number of connection received by a \ref Neuron in the \ref network
 */
  		double connectivity;
/*!
 * Average intensity of connection between \ref Neuron in the \ref Network
 */
  		double intensity;
/*!
 * Output file, where the results will be printed
 */
		std::ofstream outfile;
/*!
 * Output file, where the potential, recovery and current of some neurons will be printed
 */
		std::ofstream samplefile;

/*!
 * Output file, where the initial parameters of each neurons will be printed
 */
		std::ofstream paramfile;

};
