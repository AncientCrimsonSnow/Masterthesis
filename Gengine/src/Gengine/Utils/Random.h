#pragma once

#include <random>

namespace Gengine
{
	class Random
	{
	public:
		Random(unsigned int seed = std::random_device{}());
		void SetSeed(unsigned int seed);
		int NextInt(int min, int max);
		float NextFloat(float min, float max);
		double NextDouble(double min, double max);
		float NextFloat01();
		double NextDouble01();

	private:
		std::mt19937 gen;
	};
}


