#include "Simulation.h"

Simulation::Simulation(int argc, char **argv) {
     try {
        TCLAP::CmdLine cmd("Neuron Network simulation");
        TCLAP::ValueArg<int> neuron("n", "neuron", "Number of Neuron", false, _Numbers_ , "int");
        cmd.add(neuron);
        TCLAP::ValueArg<double> prop_e("P", "prop_e", "Proportion of Excitatory Neuron", false, _Proportion_Excitatory_, "double");
        cmd.add(prop_e);
        TCLAP::ValueArg<double> time("T", "time", "Simulation Time", false, _Simulation_Time_ , "double");
        cmd.add(time);
        TCLAP::ValueArg<double> lambda("C", "lambda", "Average Connectivity", false, _Connectivity_, "double");
        cmd.add(lambda);
        TCLAP::ValueArg<double> intensity("L", "Intensity", "Average intensity of connections", false, _Intensity_, "double");
        cmd.add(intensity);
        TCLAP::ValueArg<std::string> ofile("o", "outptut", "output file name", false, "", "string");
        cmd.add(ofile);

        cmd.parse(argc, argv);

        std::string outfname = ofile.getValue();
        if (outfname.length()) outfile.open(outfname, std::ios_base::out);

        endtime=time.getValue();

       if ((prop_e.getValue() <0) or (prop_e.getValue()>1)) throw(std::runtime_error("Proportion of Excitatory Neurons must be between 0 and 1"));
       if (time.getValue() <= 0) throw(std::runtime_error("Simulation Time must be positive"));
       if (lambda.getValue() <= 0) throw(std::runtime_error("Average Connectivity must be positive"));
       if (neuron.getValue() <= 0) throw(std::runtime_error("Number of Neurons must be positive")) ;

     } catch (std::runtime_error &e) { std::cout<<e.what()<<std::endl; };
}

void Simulation::header() {
      std::ostream *outstr = &std::cout;
      if (outfile.is_open()) outstr = &outfile;
      *outstr << "time";
      for (size_t time=0; time<=endtime; time++)
          *outstr << "\t" << time ;
      *outstr << std::endl;
}

void Simulation::print(){
      header();

      std::ostream *outstr = &std::cout;
      if (outfile.is_open()) outstr = &outfile;

      for (auto n : network.get_neurons()) {
        for(double time(0); time<=endtime ; time++){
            if (network.neuron_firing(n)){
                *outstr << "\t" << '1' ;
            } else {
                *outstr << "\t" << '0' ;
            }
          }
          *outstr << std::endl;
      }
}

void Simulation::run() {
	
}

Simulation::~Simulation() {

}
