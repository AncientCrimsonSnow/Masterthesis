#include "ggpch.h"
#include "NeuralNetwork.h"
namespace Gengine
{
	NeuralNetworkData::NeuralNetworkData(
		const Vector<int>& layerSizes, 
		const ActivationFunctionType activationType, 
		const ActivationFunctionType activationTypeOutput) :
		m_ActivationType(activationType),
		m_ActivationTypeOutput(activationTypeOutput)
	{
		GG_CORE_ASSERT(layerSizes.size() < 2, "At least 2 Layers must be defined for an input and an output");

		for (size_t i = 1; i < layerSizes.size(); ++i)
			m_Layers.push_back(NeuralNetworkLayer(layerSizes[i], layerSizes[i - 1]));
	}

	nlohmann::json NeuralNetworkData::GetJson() const
	{
		nlohmann::json j;

		j["activation_type"] = m_ActivationType;
		j["activation_type_output"] = m_ActivationTypeOutput;
		j["input_layer_size"] = GetInputLayerSize();

		for (const var& layer : m_Layers)
		{
			nlohmann::json layerJson;
			layerJson["weights"] = layer.weights;
			layerJson["biases"] = layer.biases;
			j["layers"].push_back(layerJson);
		}

		return j;
	}

	NeuralNetworkData NeuralNetworkData::CreateInstanceFromJson(nlohmann::json j) const
	{
		ActivationFunctionType activationType = j["activation_type"];
		ActivationFunctionType activationTypeOutput = j["activation_type_output"];

		int inputLayerSize = j["input_layer_size"];
		Vector<int> layerSizes;
		layerSizes.push_back(inputLayerSize);

		for (const var& layerJson : j["layers"])
		{
			int numNeurons = layerJson["biases"].size();
			layerSizes.push_back(numNeurons);
		}

		NeuralNetworkData result(layerSizes, activationType, activationTypeOutput);

		for (size_t i = 0; i < result.m_Layers.size(); ++i)
		{
			Vector<Vector<double>> weights;
			for (const var& weightRow : j["layers"][i]["weights"])
			{
				Vector<double> row = weightRow.get<Vector<double>>();
				weights.push_back(row);
			}

			result.m_Layers[i].weights = weights;
			result.m_Layers[i].biases = j["layers"][i]["biases"].get<Vector<double>>();
		}

		return result;
	}

	NeuralNetwork::NeuralNetwork(const NeuralNetworkData data) :
		m_Data(data),
		m_ActivationFunction(ActivationFunction::CreateActivationFunction(m_Data.GetActivationType())),
		m_ActivationFunctionOutput(ActivationFunction::CreateActivationFunction(m_Data.GetActivationTypeOutput()))
	{}

	NeuralNetwork::NeuralNetwork(
		const Vector<int>& layerSizes,
		const ActivationFunctionType activationType,
		const ActivationFunctionType activationTypeOutput) :
		m_Data(layerSizes, activationType, activationTypeOutput),
		m_ActivationFunction(ActivationFunction::CreateActivationFunction(m_Data.GetActivationType())),
		m_ActivationFunctionOutput(ActivationFunction::CreateActivationFunction(m_Data.GetActivationTypeOutput()))
	{}

	Vector<double> NeuralNetwork::Inference(const Vector<double>& input)
	{
		Vector<double> currentInput = input;
		int layerCount = m_Data.GetLayerCount();
		for (size_t layerIndex = 0; layerIndex < layerCount; ++layerIndex)
		{
			NeuralNetworkLayer& layer = m_Data.GetLayer(layerIndex);
			Vector<double> newInput(layer.neurons.size());

			for (size_t i = 0; i < layer.neurons.size(); ++i)
			{
				double sum = 0.0;
				for (size_t j = 0; j < layer.weights[i].size(); ++j)
					sum += layer.weights[i][j] * currentInput[j];

				sum += layer.biases[i];

				if (layerIndex == layerCount - 1)
					newInput[i] = m_ActivationFunctionOutput->Activate(sum);
				else
					newInput[i] = m_ActivationFunction->Activate(sum);
			}
			currentInput = newInput;
		}
		return currentInput;
	}
	bool NeuralNetwork::TrySave(const string& fileName, const string& dirPath) const
	{
		return m_Data.TrySave(fileName, dirPath);
	}
	size_t NeuralNetwork::GetInputSize() const
	{
		return m_Data.GetInputLayerSize();
	}
	NeuralNetworkLayer& NeuralNetwork::GetLayer(int index)
	{
		return m_Data.GetLayer(index);
	}
	size_t NeuralNetwork::GetLayerCount() const
	{
		return m_Data.GetLayerCount();
	}
}
