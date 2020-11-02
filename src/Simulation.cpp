#include "constants.h"
#include "Simulation.h"

Simulation::Simulation(int argc, char **argv) {

        TCLAP::CmdLine cmd("Neuron Network simulation");
        TCLAP::ValueArg<int> neuron("n", "neuron", "Number of Neuron", false, _Numbers_ , "int");
        cmd.add(neuron);
        TCLAP::ValueArg<double> prop_e("Pe", "prop_e", "Proportion of Excitatory Neuron", false, _Proportion_Excitatory_, "double");
        cmd.add(prop_e);
        TCLAP::ValueArg<double> time("T", "time", "Simulation Time", false, _Simulation_Time_ , "double");
        cmd.add(time);
        TCLAP::ValueArg<double> lambda("C", "lambda", "Average Connectivity", false, _Connectivity_, "double");
        cmd.add(lambda);
        TCLAP::ValueArg<double> intensity("L", "Intensity", "Average intensity of connections", false, _Intensity_, "double");
        cmd.add(intensity);

        cmd.parse(argc, argv);


       if (neuron.getValue() <= 0) throw(std::runtime_error("Number of Neurons must be positive"));
       if ((prop_e.getValue() <0) or (prop_e.getValue()>1)) throw(std::runtime_error("Proportion of Excitatory Neurons must be between 0 and 1"));
       if (time.getValue() <= 0) throw(std::runtime_error("Simulation Time must be positive"));
       if (lambda.getValue() <= 0) throw(std::runtime_error("Average Connectivity must be positive"));
}

Simulation::~Simulation() {

}
