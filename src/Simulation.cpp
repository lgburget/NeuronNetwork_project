#include "Simulation.h"

Simulation::Simulation(int argc, char **argv)
{
    // setting the allowed values for the model passed by the user
     allowed.push_back("constant");
     allowed.push_back("poisson");
     allowed.push_back("over-dispersed");
     TCLAP::ValuesConstraint<std::string> allowed_models(allowed);

     try {
		// get the parameter in the command line
        TCLAP::CmdLine cmd("Neuron Network simulation");
        TCLAP::ValueArg<int> neuron("n", "number", "Number of Neuron", false, _Numbers_ , "int");
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
        n_types = types.getValue();
        d = delta.getValue();
        model = connectivity_model.getValue();

        // Creation of the neuron network
        network = new Network(number, n_types, d, connectivity, model, intensity);

     } catch (std::runtime_error &e) {
       std::cout<<e.what()<<std::endl;
       exit(EXIT_FAILURE);
     } ;
}

void Simulation::run()
{
	// this will be called once, at the beginning of the simulation
	std::ostream *outstr_param=nullptr;
	std::ostream *outstr_sample=nullptr;
	std::ostream *outstr_print=nullptr;
    if (paramfile.is_open()) outstr_param = &paramfile;
    if (samplefile.is_open()) outstr_sample = &samplefile;
    if (outfile.is_open()) outstr_print = &outfile;
    network->header_sample(outstr_sample);								// print a header in sample file
    network->print_parameters(outstr_param);							// print parameters of every neuron
	// for each step of the simulation, first the network is updated by updating each neurons of the network
	// then the results are printed in the output files
	for (int t(1); t<=endtime; ++t) {
		std::vector<size_t> firing_n = network->update();
		*outstr_print << t;
		for (size_t i(0); i < number; ++i) *outstr_print << " " << std::count(firing_n.begin(), firing_n.end(), i);
		*outstr_print << std::endl;
		network->print_sample(t, outstr_sample);
	}

	// the output files are closed
	if (outfile.is_open()) outfile.close();
	if (samplefile.is_open()) samplefile.close();
	if (paramfile.is_open()) paramfile.close();
}

Simulation::~Simulation()
{
	delete network;
}
