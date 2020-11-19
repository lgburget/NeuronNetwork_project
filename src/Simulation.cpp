#include "Simulation.h"

Simulation::Simulation(int argc, char **argv) 
{
     try {
		// get the parameter in the command line
        TCLAP::CmdLine cmd("Neuron Network simulation");
        TCLAP::ValueArg<int> neuron("n", "number", "Number of Neuron", false, _Numbers_ , "int");
        cmd.add(neuron);
        TCLAP::ValueArg<double> prop_e("P", "prop_e", "Proportion of Excitatory Neuron", false, _Proportion_Excitatory_, "double");
        cmd.add(prop_e);
        TCLAP::ValueArg<int> time("t", "time", "Total simulation Time", false, _Simulation_Time_ , "int");
        cmd.add(time);
        TCLAP::ValueArg<double> lambda("c", "lambda", "Average Connectivity", false, _Connectivity_, "double");
        cmd.add(lambda);
        TCLAP::ValueArg<double> intens("l", "Intensity", "Average intensity of connections", false, _Intensity_, "double");
        cmd.add(intens);
        TCLAP::ValueArg<std::string> ofile("o", "outptut", "output file name", false, "", "string");
        cmd.add(ofile);

        cmd.parse(argc, argv);

		//Check the values of parameters get in the command line
        if ((prop_e.getValue() <0) or (prop_e.getValue()>1)) throw(std::runtime_error("Proportion of Excitatory Neurons must be between 0 and 1"));
        if (time.getValue() <= 0) throw(std::runtime_error("Simulation Time must be positive"));
        if (lambda.getValue() <= 0) throw(std::runtime_error("Average Connectivity must be positive"));
        if (neuron.getValue() <= 0) throw(std::runtime_error("Number of Neurons must be positive")) ;
       
        // creation of an output file
        std::string outfname = ofile.getValue();
        if (outfname.length()) outfile.open(outfname, std::ios_base::out);
        
        // Setting of parameters into attributs if simulation
        endtime = time.getValue();
        number = neuron.getValue();
        prop_exc = prop_e.getValue();
        connectivity = lambda.getValue();
        intensity = intens.getValue();
        
        // Creation of the neuron network
        network = new Network(number, prop_exc, connectivity, intensity);

     } catch (std::runtime_error &e) { std::cout<<e.what()<<std::endl; };
}

void Simulation::header() 
{
      std::ostream *outstr = &std::cout;
      if (outfile.is_open()) outstr = &outfile;
      *outstr << "neuron" << std::endl << "time";   
      for (size_t n=1; n<=number; n++)
          *outstr << "\t" << n ;
      *outstr << std::endl;
}

void Simulation::print(const int& i)
{
      std::ostream *outstr = &std::cout;
      if (outfile.is_open()) outstr = &outfile;
      
      *outstr << i;								// display of the simulation time step
      for (auto n : network->get_neurons()) {
            if (network->neuron_firing(n)){
                *outstr << "\t" << '1' ;
            } else {
                *outstr << "\t" << '0' ;
            }
      }
      *outstr << std::endl;
}

void Simulation::run() 
{
	for (int i(1); i<=endtime; ++i) {
		network->update();
		this->print(i);
	}
}

Simulation::~Simulation() 
{
	delete network;
}
