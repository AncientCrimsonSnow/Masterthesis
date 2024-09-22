#include <functional> 
#include "NeuralNetwork.h"
#include "IInputConverter.h"

namespace Gengine
{
    template<typename TInput>
    class NeuralNetworkTrainer
    {
    public:
        NeuralNetworkTrainer() = default;
        NeuralNetworkTrainer(
            std::unique_ptr<NeuralNetwork> model,
            std::unique_ptr<IInputConverter<TInput>> inputConverter);

        NeuralNetworkTrainer(
            NeuralNetworkData modelData,
            std::unique_ptr<IInputConverter<TInput>> inputConverter);

        NeuralNetworkTrainer(
            const Vector<int>& layerSizes,
            const ActivationFunctionType activationType,
            const ActivationFunctionType activationTypeOutput,
            std::unique_ptr<IInputConverter<TInput>> inputConverter);

        bool TryConfigureAndTrain(
            int iterations,
            double learningRate,
            std::function<double(const TInput&, const Vector<double>&)> rewardFunc,
            const TInput input,
            int saveInterval);

        bool TryConfigureAndTrain(
            int iterations,
            double learningRate,
            std::function<double(const TInput&, const Vector<double>&)> rewardFunc,
            const Vector<TInput>& input)
        {
            return TryConfigureAndTrain(
                iterations,
                learningRate,
                rewardFunc,
                input,
                -1);
        }

        bool TryConfigureAndTrain(
            int iterations,
            double learningRate,
            std::function<double(const TInput&, const Vector<double>&)> rewardFunc,
            const Vector<TInput> trainingsData,
            int saveInterval);

        bool TryConfigureAndTrain(
            int iterations,
            double learningRate,
            std::function<double(const TInput&, const Vector<double>&)> rewardFunc,
            const Vector<TInput> trainingsData)
        {
            return TryConfigureAndTrain(
                iterations,
                learningRate,
                rewardFunc,
                trainingsData,
                -1);
        }


        bool TryConfigureAndTrain(
            int iterations,
            double learningRate,
            std::function<double(const TInput&, const Vector<double>&)> rewardFunc,
            std::function<TInput()> trainingsDataGeneratorFunction,
            int saveInterval);

        bool TryConfigureAndTrain(
            int iterations,
            double learningRate,
            std::function<double(const TInput&, const Vector<double>&)> rewardFunc,
            std::function<TInput()> trainingsDataGeneratorFunction)
        {
            return TryConfigureAndTrain(
                iterations,
                learningRate,
                rewardFunc,
                trainingsDataGeneratorFunction,
                -1);
        }

        std::shared_ptr<NeuralNetwork> GetModel() const
        {
            return m_Model;
        }

        void StopTraining()
        {
            m_IsRunning = false;
        }

    private:
        std::unique_ptr<NeuralNetwork> m_Model;
        std::unique_ptr<IInputConverter<TInput>> m_InputConverter;
        bool m_IsRunning;

        void Train(
            const TInput& input,
            double learningRate,
            std::function<double(const TInput&, const Vector<double>&)> rewardFunc);

        void Autosafe(
            int iteration,
            const TInput& input,
            std::function<double(const TInput&, const Vector<double>&)> rewardFunc) const;

        bool CheckInputSize(Vector<double>& input) const;

        Vector<Vector<double>> Backpropagate(
            Vector<double>& errorGradients,
            Vector<double>& input,
            double learningRate);

        Vector<double> ComputeErrorGradient(const Vector<double>& output, double reward) const;
    };

    template<typename TInput>
    NeuralNetworkTrainer<TInput>::NeuralNetworkTrainer(
        std::unique_ptr<NeuralNetwork> model,
        std::unique_ptr<IInputConverter<TInput>> inputConverter) :
        m_Model(model),
        m_InputConverter(inputConverter)
    {}

    template<typename TInput>
    NeuralNetworkTrainer<TInput>::NeuralNetworkTrainer(
        NeuralNetworkData modelData,
        std::unique_ptr<IInputConverter<TInput>> inputConverter) :
        m_Model(std::make_unique<NeuralNetwork>(modelData)),
        m_InputConverter(inputConverter)
    {}

    template<typename TInput>
    NeuralNetworkTrainer<TInput>::NeuralNetworkTrainer(
        const Vector<int>& layerSizes,
        const ActivationFunctionType activationType,
        const ActivationFunctionType activationTypeOutput,
        std::unique_ptr<IInputConverter<TInput>> inputConverter) :
        m_Model(std::make_unique<NeuralNetwork>(layerSizes, activationType, activationTypeOutput)),
        m_InputConverter(std::move(inputConverter))
    {}

    template<typename TInput>
    bool NeuralNetworkTrainer<TInput>::TryConfigureAndTrain(
        int iterations,
        double learningRate,
        std::function<double(const TInput&, const Vector<double>&)> rewardFunc,
        const TInput input,
        int saveInterval)
    {
        m_IsRunning = true;
        if (saveInterval == -1)
            saveInterval = iterations;

        Vector<double> convertedInput = m_InputConverter->Convert(input);

        if (!CheckInputSize(convertedInput))
        {
            GG_CORE_WARN("Input TrainingsData is not in the correct format");
            return false;
        }

        for (int i = 0; i < iterations; )
        {
            for (int j = 0; j < saveInterval && i < iterations; ++j, ++i)
                if (!m_IsRunning)
                {
                    Autosafe(
                        i,
                        input,
                        rewardFunc);
                    return true;
                }
                Train(
                    input,
                    learningRate,
                    rewardFunc);
            Autosafe(
                i,
                input,
                rewardFunc);
        }
        return true;
    }

    template<typename TInput>
    bool NeuralNetworkTrainer<TInput>::TryConfigureAndTrain(
        int iterations,
        double learningRate,
        std::function<double(const TInput&, const Vector<double>&)> rewardFunc,
        const Vector<TInput> trainingsData,
        int saveInterval)
    {
        m_IsRunning = true;
        if (saveInterval == -1)
            saveInterval = iterations;

        Random random;

        for (TInput inputData : trainingsData)
        {
            Vector<double> convertedInput = m_InputConverter->Convert(inputData);
            if (!CheckInputSize(convertedInput))
            {
                GG_CORE_WARN("Input TrainingsData is not in the correct format");
                return false;
            }
        }

        for (int i = 0; i < iterations; )
        {
            for (int j = 0; j < saveInterval && i < iterations; ++j, ++i)
            {
                int randomIndex = random.NextInt(0, trainingsData.size() - 1);
                const Vector<double>& selectedInput = m_InputConverter->Convert(trainingsData[randomIndex]);
                if (!m_IsRunning)
                {
                    Autosafe(
                        i,
                        selectedInput,
                        rewardFunc);
                    return true;
                }
                Train(
                    selectedInput,
                    learningRate,
                    rewardFunc);
            }
            int randomIndex = random.NextInt(0, trainingsData.size() - 1);
            TInput selectedInput = trainingsData[randomIndex];
            Autosafe(
                i,
                selectedInput,
                rewardFunc);
        }
        return true;
    }

    template<typename TInput>
    bool NeuralNetworkTrainer<TInput>::TryConfigureAndTrain(
        int iterations,
        double learningRate,
        std::function<double(const TInput&, const Vector<double>&)> rewardFunc,
        std::function<TInput()> trainingsDataGeneratorFunction,
        int saveInterval)
    {
        m_IsRunning = true;
        if (saveInterval == -1)
            saveInterval = iterations;

        TInput generatedData = trainingsDataGeneratorFunction();
        Vector<double> convertedInput = m_InputConverter->Convert(generatedData);
        if (!CheckInputSize(convertedInput))
        {
            GG_CORE_WARN("Input TrainingsData is not in the correct format");
            return false;
        }

        for (int i = 0; i < iterations; )
        {
            for (int j = 0; j < saveInterval && i < iterations; ++j, ++i)
            {
                if (!m_IsRunning)
                {
                    Autosafe(
                        i,
                        generatedData,
                        rewardFunc);
                    return true;
                }
                    
                generatedData = trainingsDataGeneratorFunction();
                Train(
                    generatedData,
                    learningRate,
                    rewardFunc);
            }

            generatedData = trainingsDataGeneratorFunction();
            Autosafe(
                i,
                generatedData,
                rewardFunc);
        }
        return true;
    }

    template<typename TInput>
    void NeuralNetworkTrainer<TInput>::Train(
        const TInput& input,
        double learningRate,
        std::function<double(const TInput&, const Vector<double>&)> rewardFunc)
    {
        Vector<double> convertedInput = m_InputConverter->Convert(input);
        Vector<double> output = m_Model->Inference(convertedInput);
        double reward = rewardFunc(input, output);

        Vector<double> errorGradient = ComputeErrorGradient(
            output, 
            reward);
        Backpropagate(
            errorGradient,
            convertedInput,
            learningRate);
    }

    template<typename TInput>
    void NeuralNetworkTrainer<TInput>::Autosafe(
        int iteration,
        const TInput& input,
        std::function<double(const TInput&, const Vector<double>&)> rewardFunc) const
    {
        Vector<double> output = m_Model->Inference(m_InputConverter->Convert(input));
        double reward = rewardFunc(input, output);
        m_Model->TrySave("ModelData");
        GG_INFO("Reward: {:.5f}", reward);
        GG_INFO("Iteration: {}", iteration);
    }

    template<typename TInput>
    bool NeuralNetworkTrainer<TInput>::CheckInputSize(Vector<double>& input) const
    {
        int inputSize = m_Model->GetInputSize();
        GG_CORE_INFO("Expected Input Size: {0}, Actual Input Size: {1}", inputSize, input.size());
        return input.size() == inputSize;
    }

    template<typename TInput>
    Vector<Vector<double>> NeuralNetworkTrainer<TInput>::Backpropagate(
        Vector<double>& errorGradients,
        Vector<double>& input,
        double learningRate)
    {
        Vector<Vector<double>> deltas;
        Vector<double> previousLayerErrors = errorGradients;


        size_t layerCount = m_Model->GetLayerCount();
        for (int layerIndex = layerCount - 1; layerIndex >= 0; --layerIndex)
        {
            NeuralNetworkLayer& layer = m_Model->GetLayer(layerIndex);
            Vector<double> layerInputs = (layerIndex == 0) ? Vector<double>(input.begin(), input.end())
                : Vector<double>(m_Model->GetLayer(layerIndex - 1).neurons.begin(), m_Model->GetLayer(layerIndex - 1).neurons.end());

            Vector<double> layerDeltas(layer.neurons.size());

            for (size_t i = 0; i < layer.neurons.size(); ++i)
            {
                double derivative = (layerIndex == layerCount - 1)
                    ? m_Model->m_ActivationFunctionOutput->Derivative(layer.neurons[i])
                    : m_Model->m_ActivationFunction->Derivative(layer.neurons[i]);

                layerDeltas[i] = previousLayerErrors[i] * derivative;

                for (size_t j = 0; j < layer.weights[i].size(); ++j)
                {
                    layer.weights[i][j] -= learningRate * layerDeltas[i] * layerInputs[j];
                }
                layer.biases[i] -= learningRate * layerDeltas[i];
            }

            deltas.push_back(layerDeltas);

            if (layerIndex > 0)
            {
                size_t lastLayerNeuronsSize = m_Model->GetLayer(layerIndex - 1).neurons.size();
                Vector<double> newErrors(lastLayerNeuronsSize, 0.0);
                for (size_t i = 0; i < lastLayerNeuronsSize; ++i)
                {
                    double sum = 0.0;
                    for (size_t j = 0; j < layer.neurons.size(); ++j)
                    {
                        sum += layer.weights[j][i] * layerDeltas[j];
                    }
                    newErrors[i] = sum;
                }
                previousLayerErrors = newErrors;
            }
        }
        return deltas;
    }

    template<typename TInput>
    Vector<double> NeuralNetworkTrainer<TInput>::ComputeErrorGradient(const Vector<double>& output, double reward) const
    {
        Vector<double> errorGradient(output.size());
        for (size_t i = 0; i < output.size(); ++i)
        {
            double derivative = m_Model->m_ActivationFunctionOutput->Derivative(output[i]);
            errorGradient[i] = -reward * derivative / output[i];
        }
        return errorGradient;
    }
}