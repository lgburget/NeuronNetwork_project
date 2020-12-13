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
	// type of the neuron
	n_type = type;
	
	// picking parameters a,b,c and d based on the value of Neuron_types and multiplied by a random noise
	set_params ({Neuron_types.at(type).a * _RNG->uniform_double(1.0 - delta, 1.0 + delta),
				 Neuron_types.at(type).b * _RNG->uniform_double(1.0 - delta, 1.0 + delta),
				 Neuron_types.at(type).c * _RNG->uniform_double(1.0 - delta, 1.0 + delta),
				 Neuron_types.at(type).d * _RNG->uniform_double(1.0 - delta, 1.0 + delta),
				 Neuron_types.at(type).excit});
	set_potential(-65);
	set_recovery (params_.b*pot_);
}

void Neuron::equation()
{
	pot_ += 0.5*(0.04*pot_*pot_+5*pot_+140-rec_+curr_);
	pot_ += 0.5*(0.04*pot_*pot_+5*pot_+140-rec_+curr_);
	rec_ += params_.a*(params_.b*pot_-rec_);
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
