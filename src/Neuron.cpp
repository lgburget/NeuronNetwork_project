#include "Neuron.h"

Neuron::Neuron()
{};

Neuron::Neuron(bool excit) : curr_(0.0)
{
	double r = _RNG->uniform_double(0,1);
	if (excit) {
		double a = 0.02;
		double b = 0.2;
		double c = -65*(1 - (3*r*r/13));
		double d = 8*(1 - (3*r*r/4));
		params_ = {a,b,c,d,excit};
		pot_ = -65;
		rec_ = b*pot_;
	} else {
		double a = 0.1*(1 - (0.8*r));
		double b = 0.2*(1 + (0.25*r));
		double c = -65;
		double d = 2;
		params_ = {a,b,c,d,excit};
		pot_ = -65;
		rec_ = b*pot_;
	}
	
}

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
