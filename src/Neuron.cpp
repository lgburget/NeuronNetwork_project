#include "Neuron.h"

const std::map<std::string, Neuron_parameters> Neuron::Neuron_types {
	{"RS",  {.02, .2,  -65, 8,   true}},
    {"IB",  {.02, .2,  -55, 4,   true}},
    {"FS",  {.1,  .2,  -65, 2,   false}},
    {"LTS", {.02, .25, -65, 2,   false}},
    {"CH",  {.02, .2,  -50, 2,   true}}
};

Neuron::Neuron(const std::string &type, const double &delta) : curr_(0.0)
{
	// type proportions
	n_type = type;
	
	// picking parameters a,b,c and d based on the value of Neuron_types and multiplied by a random noise
	double a,b,c,d;
	double noise = _RNG->uniform_double(1.0 - delta, 1.0 + delta);
	a = Neuron_types.at(type).a * noise;
	noise = _RNG->uniform_double(1.0 - delta, 1.0 + delta);
	b = Neuron_types.at(type).b * noise;
	noise = _RNG->uniform_double(1.0 - delta, 1.0 + delta);
	c = Neuron_types.at(type).c * noise;
	noise = _RNG->uniform_double(1.0 - delta, 1.0 + delta);
	d = Neuron_types.at(type).d * noise;
	
	// type of neuron = inhibitory or excitatory
	bool excit = Neuron_types.at(type).excit;
	
	// setting the parameters and 
	this->set_params ({a,b,c,d,excit});				// set the parameters of neuron
	this->set_potential(-65);
	this->set_recovery (b*pot_);
}

void Neuron::equation()
{
	pot_ += _Delta_T_*(0.04*pot_*pot_+5*pot_+140-rec_+curr_);
	pot_ += _Delta_T_*(0.04*pot_*pot_+5*pot_+140-rec_+curr_);
	rec_ += 2*_Delta_T_*params_.a*(params_.b*pot_-rec_);
}

std::string Neuron::params_to_print() const
{
	std::stringstream ss;
	ss << n_type
	   << "\t" << params_.a
	   << "\t" << params_.b
	   << "\t" << params_.c
	   << "\t" << params_.d
	   << "\t" << (int)params_.excit;									// returns 1 if true and 0 if false.
	return ss.str();
}

std::string Neuron::variables_to_print() const 
{
	std::stringstream ss;
	ss << "\t" << pot_ << "\t" << rec_ << "\t" << curr_;
	return ss.str();
}
