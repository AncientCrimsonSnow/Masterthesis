#pragma once

#include "NeuralNetworkLayer.h"
#include "Gengine/Artificial Intelligence/Deep Learning/Activation Function/ActivationFunction.h"
#include "Gengine/Artificial Intelligence/DeepLearningArtificialIntelligence.h"
#include "Gengine/Utils/Persistence.h"

namespace Gengine
{
	class NeuralNetworkData : public Persistence<NeuralNetworkData>
	{
	public:
		NeuralNetworkData() = default;
		NeuralNetworkData(
			const Vector<int>& layerSizes,
			const ActivationFunctionType activationType,
			const ActivationFunctionType activationTypeOutput);

		const ActivationFunctionType GetActivationType() const
		{
			return m_ActivationType;
		}

		const ActivationFunctionType GetActivationTypeOutput() const
		{
			return m_ActivationTypeOutput;
		}

		int GetLayerCount() const
		{
			return m_Layers.size();
		}

		NeuralNetworkLayer& GetLayer(int index)
		{
			return m_Layers[index];
		}

		size_t GetInputLayerSize() const
		{
			return m_Layers[0].weights[0].size();
		}

	protected:
		nlohmann::json GetJson() const override;
		NeuralNetworkData CreateInstanceFromJson(nlohmann::json j) const override;
	private:
		Vector<NeuralNetworkLayer> m_Layers;
		ActivationFunctionType m_ActivationType;
		ActivationFunctionType m_ActivationTypeOutput;
	};

	class NeuralNetwork : public DeepLearningArtificialIntelligence
	{
	public:
		NeuralNetwork(const NeuralNetworkData data);

		NeuralNetwork(
			const Vector<int>& layerSizes,
			const ActivationFunctionType activationType,
			const ActivationFunctionType activationTypeOutput);

		Vector<double> Inference(const Vector<double>& input) override;
		bool TrySave(const string& fileName, const string& dirPath = string()) const;
		size_t GetInputSize() const;
		NeuralNetworkLayer& GetLayer(int index);
		size_t GetLayerCount() const;

	private:
		NeuralNetworkData m_Data;
	public:
		const std::unique_ptr<ActivationFunction> m_ActivationFunction;
		const std::unique_ptr<ActivationFunction> m_ActivationFunctionOutput;
	};
}