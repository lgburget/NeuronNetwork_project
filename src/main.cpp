#include "Simulation.h"

RandomNumbers *_RNG;

/*! \mainpage NeuronNetwork
 *
 * \section intro_sec What is the goal of this program ?
 *
 * This \b NeuronNetwork program allows the user to \b model a neuron \ref Network. 
 * 
 * A \ref Neuron can be in two different state, depending on its membrane potential. 
 * Either it is in \b firing \b state, when the potential is more than a certain value, 
 * or it is not. 
 * 
 * The goal is to make a \ref Simulation for a given duration of the interactions 
 * between neurons, in order to spot, for each time step, the neurons that are in the \b firing \b state.
 *
 * \section output_sec What is the output ?
 *
 * This program produce an output file made of a \b matrix of 0 and 1. 
 * The 1 corresponds to neurons that are \b firing, while 0 corresponds to other neurons. 
 * Time is represented by the differents lines while the columns state for the differents neurons. 
 * 
 * The output, which is the matrix, can then be computed into a \b raster \b plot where points 
 * represents \b firing neurons using an other program.
 * 
 * \section install_sec How to use this program ?
 * 
 * In order to simulate the network, you have to provide some arguments:
 * 
 * - the \b total \b number of neurons you want in your network
 * - the \b total \b time you want your simulation to last
 * - the \b average \b number \b of \b connection a random neuron have
 * - the \b average \b intensity \b of \b connection between two random neurons
 * - the \b fraction \b of \b excitatory \b neurons within the network
 * - the \b file in which the results will be printed
 * 
 * If you don't provide these arguments, default parameters will be taken. 
 * 
 *
 */
 
int main(int argc, char **argv) {
	_RNG = new RandomNumbers;
	try{
		Simulation s(argc, argv);
		//s.header();
		s.run();
	} catch(SimulError &e) {
		std::cerr << e.what() << std::endl;
		return e.value();
	}
    if (_RNG) delete _RNG;
    return 0;
}

