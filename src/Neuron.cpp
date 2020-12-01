#include "Neuron.h"

const std::map<std::string, Neuron_parameters> Neuron::Neuron_types {
	{"RS",  {.02, .2,  -65, 8,   true}},
    {"IB",  {.02, .2,  -55, 4,   true}},
    {"FS",  {.1,  .2,  -65, 2,   false}},
    {"LTS", {.02, .25, -65, 2,   false}},
    {"RZ",  {.1,  .26, -65, 2,   true}}
};

Neuron::Neuron(const std::string &type, const std::vector<double> &n) : curr_(0.0)
{
	n_type = type;
	double a = Neuron_types.at(type).a * n[0];
	double b = Neuron_types.at(type).b * n[1];
	double c = Neuron_types.at(type).c * n[2];
	double d = Neuron_types.at(type).d * n[3];
	bool excit = Neuron_types.at(type).excit;
	
	this->set_params ({a,b,c,d,excit});				// set the parameters of neuron
	this->set_potential(-65);
	this->set_recovery (b*pot_);
}

void Neuron::equation()
{
	if(firing())
	{
		pot_ = params_.c;
		rec_ += params_.d;
	}else{
		pot_ += _Delta_T_*(0.04*pot_*pot_+5*pot_+140-rec_+curr_);
		pot_ += _Delta_T_*(0.04*pot_*pot_+5*pot_+140-rec_+curr_);
		rec_ += 2*_Delta_T_*params_.a*(params_.b*pot_-rec_);
	}
}
/*
void Neuron::update_pot()
{
	pot_ += _Delta_T_*((0.04*pot_*pot_) + (5*pot_) + 140 - rec_ + curr_);		
}

void Neuron::update_rec()
{
	rec_ += 2*_Delta_T_*params_.a*((params_.b*pot_) - rec_);
}

void Neuron::update_if_firing()
{
	pot_ = params_.c;
	rec_ += params_.d;
}
*/
