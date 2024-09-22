#pragma once
#include "Gengine/Utils/Vector.h"

namespace Gengine
{
    class NeuralNetworkLayer
    {
    public:
        Vector<double> neurons;
        Vector<Vector<double>> weights;
        Vector<double> biases;

        NeuralNetworkLayer() = default;

        NeuralNetworkLayer(int numNeurons, int numInputs)
        {
            neurons.resize(numNeurons);
            weights.resize(numNeurons, Vector<double>(numInputs));
            biases.resize(numNeurons);

            for (int i = 0; i < numNeurons; i++)
            {
                for (int j = 0; j < numInputs; j++)
                {
                    weights[i][j] = ((double)rand() / RAND_MAX) * 2 - 1; // Werte zwischen -1 und 1
                }
                biases[i] = ((double)rand() / RAND_MAX) * 2 - 1; // Werte zwischen -1 und 1
            }
        }
    };
}
