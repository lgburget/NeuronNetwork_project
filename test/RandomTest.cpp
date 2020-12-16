#include <gtest/gtest.h>
#include "Random.h"
#include "Neuron.h"
#include "Network.h"
#include "Simulation.h"

RandomNumbers *_RNG = new RandomNumbers(23948710923);

TEST(Random, distributions) {
    double mean = 0;
    double input_mean(1.35), input_sd(2.8);
    std::vector<double> res;
    res.resize(10000);
    double delta = input_sd*sqrt(3.0);
    double lower = input_mean-delta, upper = input_mean+delta;
    _RNG->uniform_double(res, lower, upper);
    for (auto I : res) {
        EXPECT_GE(I, lower);
        EXPECT_LT(I, upper);
        mean += I*1e-4;
    }
    EXPECT_NEAR(input_mean, mean, 3e-2*input_sd);
    _RNG->normal(res, input_mean, input_sd);
    mean = 0;
    for (auto I : res) mean += I*1e-4;
    EXPECT_NEAR(input_mean, mean, 2e-2*input_sd);
    _RNG->poisson(res, input_mean);
    mean = 0;
    for (auto I : res) mean += I*1e-4;
    EXPECT_NEAR(input_mean, mean, 2e-2*input_mean);
}

TEST(Network, Initialze) {
	Network net1(50, "RS:1.", 0.1, 5, "constant", 1);
	std::vector<Neuron> neurons1 = net1.get_neurons();
	for(auto n : neurons1)
	{
		Neuron_parameters param = n.get_params();
		EXPECT_EQ(true, param.excit);
		EXPECT_NEAR(0.02, param.a, 0.002);
		EXPECT_NEAR(0.2, param.b, 0.02);
		EXPECT_NEAR(-13, n.get_recovery(), 1.3);
	}
	Network net2(50, "FS:1.", 0.1, 5, "constant", 1);
	std::vector<Neuron> neurons2 = net2.get_neurons();
	for(auto n : neurons2)
	{
		Neuron_parameters param = n.get_params();
		EXPECT_EQ(false, param.excit);
		EXPECT_NEAR(-65, param.c, 6.5);
		EXPECT_NEAR(2, param.d, 0.2);
		EXPECT_NEAR(-13, n.get_recovery(), 1.3);
	}
	Network net3(50, "", 0.1, 0, "constant", 1);
	std::vector<Neuron> neurons3 = net3.get_neurons();
	int n_inhib = 0;
	int n_excit = 0;
	for(auto n : neurons3)
	{
		if(n.get_params().excit) ++n_excit;
		else ++n_inhib;
	}
	EXPECT_EQ(25, n_inhib);
	EXPECT_EQ(25, n_excit);
}


TEST(Network, addlink) {
	Network net(2, "RS:1", 0., 0, "", 1);
	net.add_link(0, 1, 1);
	EXPECT_EQ(1, net.get_links().count({0,1}));
	EXPECT_EQ(false, net.add_link(0,0,1));
	EXPECT_EQ(false, net.add_link(2,0,1));
}


TEST(Network, linking) {
	Network net1(100, "", 0., 5, "constant", 1);
	double mean1 = (double)net1.get_links().size()/100;
	EXPECT_EQ(mean1, 5.0);

	Network net2(100, "", 0., 5, "poisson", 1);
	double mean2 = (double)net2.get_links().size()/100;
	EXPECT_NEAR(mean2, 5.0, 1.0);
	
	Network net3(100, "", 0., 5, "over-dispersed", 1);
	double mean3 = (double)net3.get_links().size()/100;
	EXPECT_NEAR(mean3, 5.0, 1.0);		
}

TEST(Network, Neighbours) {
	Network net(4, "", 0., 0, "", 0);
	std::vector<Neuron> nn = net.get_neurons();
	net.add_link(0, 1, 0);
	net.add_link(0, 2, 0);
	net.add_link(0, 3, 0);
	std::vector<std::pair<size_t, double>> neighbours = net.find_neighbours(0);
	std::vector<std::pair<size_t, double>> expected_neighbours = {{1,0}, {2,0}, {3,0}};
	
	EXPECT_EQ(neighbours, expected_neighbours);
}

TEST(Network, potential) {
	Network net(3, "RS:1.", 0., 0, "", 1);
	std::vector<Neuron> nn = net.get_neurons();
	net.add_link(0, 1, 1);
	net.add_link(0, 2, 1);
	net.set_neuron_potential(1, 35.0);
	net.set_neuron_potential(2, 35.0);
	double i = net.total_current(0);
	EXPECT_GE(i, (-2.326*5. + 1.));
    EXPECT_LT(i, (2.326*5. + 1.));
	/*
	 * 2.326 is the 99th percentile of the normal distribution Norm(0,1)
	 * it is multiplied by 5 because the neuron 0 is excitatory
	 * 1 is added because 2 excitatory neurons send their signal to neuron 0 and have a connectivity of 1 with it
	 * So congratulations if the test failed, you are a 1 in a 100
	 */
}

TEST(Neuron, firing) {
	Neuron n1("FS", 0.);
	n1.set_potential(30.1);
	EXPECT_EQ(true, n1.firing());
	Neuron n2("RS", 0.);
	n2.set_potential(0.0);
	EXPECT_EQ(false, n2.firing());
	
	n1.reset();
	EXPECT_EQ(n1.get_params().c, n1.get_potential());
}

TEST(Network, Parsing) {
	Network net1(100, "", 0., 5, "constant", 1);
	int count_RS = 0;
	int count_FS = 0;
	for( auto n : net1.get_neurons()) {
		if(n.get_type() == "RS") ++count_RS;
		if(n.get_type() == "FS") ++count_FS;
	}
	EXPECT_EQ(50, count_RS);
	EXPECT_EQ(50, count_FS);
	
	Network net2(100, "FS:0.2, RS:0.4, LTS:0.1, CH:0.7", 0., 5, "constant", 1);
	count_RS = 0;
	count_FS = 0;
	int count_LTS = 0;
	int count_CH = 0;
	for( auto n : net2.get_neurons()) {
		if(n.get_type() == "RS") ++count_RS;
		if(n.get_type() == "FS") ++count_FS;
		if(n.get_type() == "LTS") ++count_LTS;
		if(n.get_type() == "CH") ++count_CH;
	}
	EXPECT_EQ(0, count_RS);
	EXPECT_EQ(20, count_FS);
	EXPECT_EQ(10, count_LTS);
	EXPECT_EQ(70, count_CH);
	
	Network net3(100, "FS:0.2, RS:0.4", 0., 5, "constant", 1);
	count_RS = 0;
	count_FS = 0;
	for( auto n : net3.get_neurons()) {
		if(n.get_type() == "RS") ++count_RS;
		if(n.get_type() == "FS") ++count_FS;
	}
	EXPECT_EQ(80, count_RS);
	EXPECT_EQ(20, count_FS);
}

TEST(Network, valence) {
	Network net(3, "RS:1.", 0., 0, "", 1);
	net.add_link(0, 1, 1);
	net.add_link(0, 2, 1);
	EXPECT_EQ(2, net.valence(0));
}

TEST(Network, update) {
	Network net(3, "FS:1", 0., 0, "", 1);
	net.set_neuron_potential(1,35.);
	net.add_link(0,1,1);
	double max_expected_current = (2*2.5 - 1);
	double min_expected_current = (2*(-2.5) - 1);
	double max_ex_pot = net.get_potential(0);
	double min_ex_pot = net.get_potential(0);
	
	max_ex_pot += 0.5*(0.04*max_ex_pot*max_ex_pot+5*max_ex_pot+140-net.get_recovery(0) + max_expected_current);
	max_ex_pot += 0.5*(0.04*max_ex_pot*max_ex_pot+5*max_ex_pot+140-net.get_recovery(0) + max_expected_current);
	
	min_ex_pot += 0.5*(0.04*min_ex_pot*min_ex_pot+5*min_ex_pot+140-net.get_recovery(0) + min_expected_current);
	min_ex_pot += 0.5*(0.04*min_ex_pot*min_ex_pot+5*min_ex_pot+140-net.get_recovery(0) + min_expected_current);
	
	net.update();
	EXPECT_LT(min_ex_pot, net.get_potential(0));
	EXPECT_GE(max_ex_pot, net.get_potential(0));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
