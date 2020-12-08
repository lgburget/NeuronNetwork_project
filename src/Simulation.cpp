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
        TCLAP::ValueArg<std::string> types("T", "types", "Proportions of each type", false, "", "string");
        cmd.add(types);
        TCLAP::ValueArg<double> delta("d", "delta", "delta on noise", false, _Delta_, "double");
        cmd.add(delta);
        TCLAP::ValueArg<std::string> connectivity_model("M", "model", "dispersion model", false, "poisson", &allowed_models );
        cmd.add(connectivity_model);
        TCLAP::ValueArg<int> time("t", "time", "Total simulation Time", false, _Simulation_Time_ , "int");
        cmd.add(time);
        TCLAP::ValueArg<double> lambda("c", "lambda", "Average Connectivity", false, _Connectivity_, "double");
        cmd.add(lambda);
        TCLAP::ValueArg<double> intens("l", "Intensity", "Average intensity of connections", false, _Intensity_, "double");
        cmd.add(intens);
        TCLAP::ValueArg<std::string> ofile("o", "outptut", "output file name", false, "outfile.txt", "string");
        cmd.add(ofile);
        TCLAP::ValueArg<std::string> sfile("s", "sample", "sample output file name", false, "sample_file.txt", "string");
        cmd.add(sfile);
        TCLAP::ValueArg<std::string> pfile("p", "parameters", "parameters output file name", false, "param_file.txt", "string");
        cmd.add(pfile);
        cmd.parse(argc, argv);

		//Check the values of parameters get in the command line
        if ( (delta.getValue() < 0) or (time.getValue() <= 0) or (lambda.getValue() <= 0) or (neuron.getValue() <= 0) or (intens.getValue() < 0))
        throw(std::runtime_error("Parameters are non valid."));

        // creation of output file
        std::string outfname = ofile.getValue();
        if (outfname.length()) outfile.open(outfname, std::ios_base::out);
        outfname = sfile.getValue();
        if (outfname.length()) samplefile.open(outfname, std::ios_base::out);
        outfname = pfile.getValue();
        if (outfname.length()) paramfile.open(outfname, std::ios_base::out);

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

void Simulation::print(const int& t)
{
    std::ostream *outstr = &std::cout;
    if (outfile.is_open()) outstr = &outfile;

	  *outstr << t << " ";
      for (auto n : network->get_neurons()) {
            if (network->neuron_firing(n)){
                *outstr << '1' << " ";        // if the neuron is firing, print 1 in the column of the neuron at the corresponding time
            } else {
                *outstr << '0' << " ";		// if the neuron is not firing, same but print a 0.
            }
      }
      *outstr << std::endl;
}

void Simulation::header_sample()
{
      std::ostream *outstr = &std::cout;
      if (samplefile.is_open()) outstr = &samplefile;

      if (network->is_type("RS")) *outstr << "\t" << "RS.v" << "\t" << "RS.u" << "\t" << "RS.I";
      if (network->is_type("IB")) *outstr << "\t" << "IB.v" << "\t" << "IB.u" << "\t" << "IB.I";
      if (network->is_type("FS")) *outstr << "\t" << "FS.v" << "\t" << "FS.u" << "\t" << "FS.I";
      if (network->is_type("LTS")) *outstr << "\t" << "LTS.v" << "\t" << "LTS.u" << "\t" << "LTS.I";
      if (network->is_type("CH")) *outstr << "\t" << "CH.v" << "\t" << "CH.u" << "\t" << "CH.I";
      *outstr << std::endl;
}

void Simulation::print_sample(const int& t)
{
	  std::ostream *outstr = &std::cout;
      if (samplefile.is_open()) outstr = &samplefile;

	  *outstr << t ;
	  if (network->is_type("RS")) print_properties("RS");
      if (network->is_type("IB")) print_properties("IB");
      if (network->is_type("FS")) print_properties("FS");
      if (network->is_type("LTS")) print_properties("LTS");
      if (network->is_type("CH")) print_properties("CH");
      *outstr << std::endl;
}

void Simulation::print_properties(const std::string& type)
{
	std::ostream *outstr = &std::cout;
    if (samplefile.is_open()) outstr = &samplefile;

	size_t n = network->find_first_neuron(type);
	*outstr << "\t" << network->get_potential(n) << "\t" << network->get_recovery(n) << "\t" << network->get_current(n);
}

void Simulation::print_parameters()
{
	std::ostream *outstr = &std::cout;
    if (paramfile.is_open()) outstr = &paramfile;
    
    // Print of the header
    *outstr << "Type" << "\t" << "a" << "\t" << "b" << "\t" << "c" << "\t" << "d" << "\t" << "Inhibitory" << "\t" << "degree" << "\t" << "valence";
    *outstr << std::endl;

	//Usefull variable
	char inhibitory;
	Neuron* neuron = nullptr;

    for (size_t i(0); i<network->get_neurons().size(); ++i) {
		  // Simplified notation
		  neuron = &network->get_neurons()[i];
		  
		  // Is the neuron inhibitory or excitatory 
		  if (neuron->get_params().excit) inhibitory = '0'; 
		  else inhibitory = '1';

		  // Print the parameters
		  *outstr << neuron->get_type()
		  << "\t" << neuron->get_params().a
		  << "\t" << neuron->get_params().b
		  << "\t" << neuron->get_params().c
		  << "\t" << neuron->get_params().d
		  << "\t" << inhibitory
		  << "\t" << network->find_neighbours(i).size()
		  << "\t" << network->valence(i);
		  *outstr << std::endl;
      }
    
    // The file is closed
    if (paramfile.is_open()) paramfile.close();
}

void Simulation::run()
{
	// this will be called once, at the beginning
	this->header_sample();  // print a header in sample file
    this->print_parameters();

	// for each step of the simulation, first the network is updated by updating each neurons of the network
	// then the results are printed in the output files
	for (int t(1); t<=endtime; ++t) {
		network->update();
		this->print(t);
		this->print_sample(t);
	}

	// the output files are closed
	if (outfile.is_open()) outfile.close();
	if (samplefile.is_open()) samplefile.close();
}

Simulation::~Simulation()
{
	delete network;
}
