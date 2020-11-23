#pragma once

#include "Network.h"

/*!
 * The \b Simulation class is the main class in this program. It constructs the neuron \ref Network according to user-specified parameters, and \ref run the simulation.
 *
 * Simulation results are printed in the file \ref outfile with the helper functions \ref header (prints a header a the top of data columns) and \ref 
 * print (prints, for every neuron, for every step of the simulation a 1 if the neuron is in firing state and 0 if not).
 * 
 * Time advances from 0 until it reaches \ref endtime.
 * 
 * A Simulation is made of a \ref Network composed of a given \ref number of \ref Neuron.
 * 
 * The parameters of the Simulation are:
 * - \ref endtime : total number of time-steps,
 * - \ref number : total number of neurons,
 * - \ref connectivity : average connectivity of a neuron,
 * - \ref intensity : average intensity of connections, 
 * - \ref prop_exc : fraction of excitatory neurons in the network,
 * - \ref outfile : output file
*/

class Simulation {
public:
/*! @name Initialization and destruction of the simulation
 * The constructor takes the command-line options and extracts all the private attributes from them to create the simulation.
*/
///@{
/*!
 *  First the command-line options are extracted. 
 * 
 * Their valuee are then verified and if there is no abberant value, attributes are initialized with them. 
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
 * The simulation is performed by calling \ref run, which updates the neuron \ref Network and calls \ref print at 
 * each simulation step.
 */
///@{

/*!
 * Print the results of the simulation in the output file
 */
		void print();
/*!
 * \ref run is the function that performs the simulation. It iterates on the simulation time and for each step, first updates the neuron \ref Network, then prints the results using \ref print. 
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
 * Total number of \ref Neuron in the \ref network
 */
  		size_t number;
/*!
 * Proportion of excitatory \ref Neuron in the \ref network
 */
  		double prop_exc; 
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
};
