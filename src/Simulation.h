#pragma once

#include "Random.h"
#include "Network.h"

class Simulation {
public:
Simulation(int, char **);



~Simulation();

private:
	Network network;


};
