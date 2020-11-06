#include "Neuron.h"

Neuron::Neuron()
{};

void Neuron::equation()
{
	if(firing())
	{
		pot_ = params_.c;
		rec_ += params_.d;
	}else{
		pot_ += _Delta_T_*(0.04*pot_*pot_+5*pot_+140-rec_+curr_);
		rec_ += _Delta_T_*params_.a*(params_.b*pot_-rec_);
	}
};
