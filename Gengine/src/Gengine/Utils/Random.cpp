#include "ggpch.h"
#include "Random.h"

namespace Gengine
{
    Random::Random(unsigned int seed) : gen(seed) {}

    void Random::SetSeed(unsigned int seed)
    {
        gen.seed(seed);
    }

    int Random::NextInt(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }

    float Random::NextFloat(float min, float max)
    {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

    double Random::NextDouble(double min, double max)
    {
        std::uniform_real_distribution<double> dist(min, max);
        return dist(gen);
    }

    float Random::NextFloat01()
    {
        return NextFloat(0.0f, 1.0f);
    }

    double Random::NextDouble01()
    {
        return NextDouble(0.0, 1.0);
    }
}