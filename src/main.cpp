#include "Random.h"
#include "Simulation.h"

RandomNumbers *_RNG;

int main(int argc, char **argv) {
	_RNG = new RandomNumbers;
	try{
		Simulation s(argc, argv);
		s.header();
		s.run();
	} catch(SimulError &e) {
		std::cerr << e.what() << std::endl;
		return e.value();
	}
    if (_RNG) delete _RNG;
    return 0;
}

