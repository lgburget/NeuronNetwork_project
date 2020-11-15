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
	Network net1(1000, 1, 5, 1);
	std::vector<Neuron> neurons1 = net1.get_neurons();
	for(auto n : neurons1)
	{
		Neuron_parameters param = n.get_params();
		EXPECT_EQ(true, param.excit);
		EXPECT_EQ(0.02, param.a);
		EXPECT_EQ(0.2, param.b);
		EXPECT_EQ(-13, n.get_recovery());
	}
	Network net2(1000, 0, 5, 1);
	std::vector<Neuron> neurons2 = net2.get_neurons();
	for(auto n : neurons2)
	{
		Neuron_parameters param = n.get_params();
		EXPECT_EQ(false, param.excit);
		EXPECT_EQ(-65, param.c);
		EXPECT_EQ(2, param.d);
		EXPECT_EQ(-65, n.get_potential());
	}
}


TEST(Network, addlink) {
	Network net(2, 1, 0, 1);
	std::vector<Neuron> nn = net.get_neurons();
	net.add_link(0, 1, 1);
	EXPECT_EQ(true, net.is_sending(1));
	EXPECT_EQ(false, net.is_sending(0));
	net.add_link(0,0,1);
	EXPECT_EQ(false, net.is_sending(0));
}


TEST(Network, linking) {
	Network net(1000, 0.5, 5, 1);
	std::vector<Neuron> nn = net.get_neurons();
	double mean = (double)net.get_links().size()/1000;
	EXPECT_NEAR(mean, 5.0, 1e-1);	
	// problème 
}

TEST(Network, Neighbours) {
	Network net(3, 0.5, 0, 0);
	std::vector<Neuron> nn = net.get_neurons();
	net.add_link(0, 1, 0);
	net.add_link(0, 2, 0);
	net.add_link(0, 3, 0);
	std::vector<std::pair<size_t, double>> neighbours = net.find_neighbours(0);
	std::vector<std::pair<size_t, double>> expected_neighbours = {{1,0}, {2,0}, {3,0}};
	
	EXPECT_EQ(neighbours, expected_neighbours);
}


TEST(Neuron, firing) {
	Neuron n1(false);
	n1.set_potential(30.1);
	EXPECT_EQ(true, n1.firing());
	Neuron n2(true);
	n2.set_potential(0.0);
	EXPECT_EQ(false, n2.firing());
	
	n1.update_rec();
	EXPECT_EQ(n1.get_params().c, n1.get_potential());
	
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
