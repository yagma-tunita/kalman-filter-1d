#include "noise.hpp"
#include <cmath>

namespace
{
    constexpr uint32_t kLcgMultiplier = 1664525u;
    constexpr uint32_t kLcgIncrement  = 1013904223u;
    constexpr double   kLcgModulus    = 4294967296.0;
    constexpr double   kPi            = 3.14159265358979323846;

    uint32_t gUniformSeed = 1;

    double GenerateUniform()
    {
        gUniformSeed = kLcgMultiplier * gUniformSeed + kLcgIncrement;
        return static_cast<double>(gUniformSeed) / kLcgModulus;
    }
}

void SeedRandom(uint32_t seed)
{
    if (seed == 0)
    {
        seed = 1;
    }
    gUniformSeed = seed;
}

double GaussianNoise(double mu, double sigma)
{
    double u1 = GenerateUniform();
    double u2 = GenerateUniform();

    while (u1 <= 0.0)
    {
        u1 = GenerateUniform();
    }

    double z0 = std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * kPi * u2);

    return mu + sigma * z0;
}