#include <gtest/gtest.h>
#include "Random.h"

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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
