#pragma once

#include "Random.h"
#include "Network.h"
#include "constants.h"

/*!
 * The Simulation class is the main class in this program. It constructs the neuron \ref Network according to user-specified parameters, and \ref run the simulation.
 *
 * Simulation results are printed to screen or file with the helper functions \ref header (prints a header a the top of data columns, which represent time) and \ref 
 * print (prints, for every neuron, for every step of the simulation a 1 if the neuron is in firing state and 0 if not).
 * 
 * Time advances from 0 until it reaches \ref endtime.
 * 
 * A Simulation is made of a \ref Network.
 * Simulation parameters:
 * - \ref endtime : total number of time-steps,
 * - \ref number : total number of neurons,
 * - \ref connectivity : average connectivity of a neuron,
 * - \ref intensity : average intensity of connections, 
 * - \ref prop_exc : fraction of excitatory neurons in the network,
 * - \ref outfile : output file

*/

class Simulation {
public:
/*! @name Initialization
  Simulation takes the command-line options and extracts the following parameters from them:
  * @param endtime The total time until the end of the simulation
  * @param outfile The name of the output file
*/

///@{
		Simulation(int, char **);
		~Simulation();
///@}


/*! @name Running the simulation
  The simulation is performed by calling \ref header once, then \ref run, which calls \ref print at \ref delta_t time intervals.
*/
///@{
		void header();
		void print(const int& i);
		void run();
///@}


private:
		Network* network;
  		int endtime;
  		size_t number;
  		double prop_exc, connectivity, intensity;
		std::ofstream outfile;
};
