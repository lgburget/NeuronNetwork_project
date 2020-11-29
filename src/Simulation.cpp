#include "Simulation.h"

Simulation::Simulation(int argc, char **argv)
{
     allowed.push_back("constant");
     allowed.push_back("poisson");
     allowed.push_back("over-dispersed");
     TCLAP::ValuesConstraint<std::string> allowed_models(allowed);
     
     try {
		// get the parameter in the command line
        TCLAP::CmdLine cmd("Neuron Network simulation");
        TCLAP::ValueArg<size_t> neuron("n", "number", "Number of Neuron", false, _Numbers_ , "size_t");
        cmd.add(neuron);
        //
        TCLAP::ValueArg<std::string> types("T", "types", "Proportions of each type", false, "", "string");
        cmd.add(types);
        TCLAP::ValueArg<double> delta("d", "delta", "delta on noise", false, _Delta_, "double");
        cmd.add(delta);
        TCLAP::ValueArg<std::string> connectivity_model("M", "model", "dispersion model", false, "poisson", &allowed_models );
        cmd.add(connectivity_model);
        //
        TCLAP::ValueArg<int> time("t", "time", "Total simulation Time", false, _Simulation_Time_ , "int");
        cmd.add(time);
        TCLAP::ValueArg<double> lambda("c", "lambda", "Average Connectivity", false, _Connectivity_, "double");
        cmd.add(lambda);
        TCLAP::ValueArg<double> intens("l", "Intensity", "Average intensity of connections", false, _Intensity_, "double");
        cmd.add(intens);
        TCLAP::ValueArg<std::string> ofile("o", "outptut", "output file name", false, "outfile.txt", "string");
        cmd.add(ofile);

        cmd.parse(argc, argv);

		//Check the values of parameters get in the command line
        if ( (delta.getValue() < 0) or (time.getValue() <= 0) or (lambda.getValue() <= 0) or (neuron.getValue() <= 0) or (intens.getValue() < 0))
        throw(std::runtime_error("Parameters are non valid."));

        // creation of an output file
        std::string outfname = ofile.getValue();
        if (outfname.length()) outfile.open(outfname, std::ios_base::out);

        // Setting of parameters into attributs if simulation
        endtime = time.getValue();
        number = neuron.getValue();
        connectivity = lambda.getValue();
        intensity = intens.getValue();
        //
        n_types = types.getValue();
        d = delta.getValue();
        model = connectivity_model.getValue();
        //

        // Creation of the neuron network
        network = new Network(number, n_types, d, connectivity, model, intensity);

     } catch (std::runtime_error &e) {
       std::cout<<e.what()<<std::endl;
       exit(EXIT_FAILURE);
     } ;
}


void Simulation::print()
{
      std::ostream *outstr = &std::cout;
      if (outfile.is_open()) outstr = &outfile;

      for (auto n : network->get_neurons()) {
            if (network->neuron_firing(n)){
                *outstr << '1' << "\t";        // if the neuron is firing, print 1 in the column of the neuron at the corresponding time
            } else {
                *outstr << '0' << "\t";		// if the neuron is not firing, same but print a 0.
            }
      }
      *outstr << std::endl;
}

void Simulation::run()
{
	// for each step of the simulation, first the network is updated by updating each neurons of the network
	// then the results are printed in the output file
	for (int i(1); i<=endtime; ++i) {
		network->update();
		this->print();
	}

	// the output file is closed
	if (outfile.is_open()) outfile.close();
}

Simulation::~Simulation()
{
	delete network;
}
