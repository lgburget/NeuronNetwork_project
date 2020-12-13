#include "Simulation.h"

RandomNumbers *_RNG;

/*! \mainpage NeuronNetwork
 *
 * \section intro_sec What is the goal of this program ?
 *
 * This \b Neuron \b Network program allows the user to \b model a neuron \ref Network. 
 * 
 * There are five different types of \ref Neuron in the \ref Network, whose proportions are choosen by the users.
 * Depending on its type, a \ref Neuron has different cellular properties.
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
 * In addition, two other output files are generated, printing the parameters of each neurons and the 
 * properties values of a sample of neurons at each time of the \ref Simulation.
 * 
 * \section install_sec How to use this program ?
 * 
 * In order to simulate the network, you have to provide some arguments:
 * 
 * - the \b total \b number of neurons you want in your network
 * - the \b total \b time you want your simulation to last
 * - the \b average \b number \b of \b connection a random neuron have
 * - the \b average \b intensity \b of \b connection between two random neurons
 * - the \b dispersion \b model for number of connection
 * - the \b proportion \b of \b each \b type \b of \b neurons within the network
 * - the length of the \b intervall \b for \b neuron \b parameters to be picked at random in
 * - the three \b files in which the results will be printed
 * 
 * If you don't provide these arguments, default parameters will be taken. 
 *
 */
 
int main(int argc, char **argv) {
	_RNG = new RandomNumbers;
	try{
		Simulation s(argc, argv);
		s.run();
	} catch(SimulError &e) {
		std::cerr << e.what() << std::endl;
		return e.value();
	}
    if (_RNG) delete _RNG;
    return 0;
}

