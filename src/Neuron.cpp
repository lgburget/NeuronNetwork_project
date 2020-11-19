#include "Neuron.h"

Neuron::Neuron(bool excit) : curr_(0.0)
{
	double r = _RNG->uniform_double(0,1);    // picking a random value
	double a,b,c,d;							 // parameters necessary to the initialisation of a neuron
	if (excit) {
		a = 0.02;
		b = 0.2;
		c = -65*(1 - (3*r*r/13));
		d = 8*(1 - (3*r*r/4));
	} else {
		a = 0.1*(1 - (0.8*r));
		b = 0.2*(1 + (0.25*r));
		c = -65;
		d = 2;
	}
	params_ = {a,b,c,d,excit};				// set the parameters of neuron
	pot_ = -65;
	rec_ = b*pot_;
	
}

/*void Neuron::equation()
{
	if(firing())
	{
		pot_ = params_.c;
		rec_ += params_.d;
	}else{
		pot_ += _Delta_T_*(0.04*pot_*pot_+5*pot_+140-rec_+curr_);
		rec_ += _Delta_T_*params_.a*(params_.b*pot_-rec_);
	}
}*/

void Neuron::update_pot()
{
	pot_ += _Delta_T_*(0.04*pot_*pot_+5*pot_+140-rec_+curr_);		
}

void Neuron::update_rec()
{
	if (firing()) {												// if the neuron is firing, potentiel and recovery are reset
		pot_ = params_.c;
		rec_ += params_.d;
	} else rec_ += 2*_Delta_T_*params_.a*(params_.b*pot_-rec_);
}
