#include "ggpch.h"
#include "ActivationFunction.h"
namespace Gengine
{
    class SigmoidActivation : public ActivationFunction
    {
    public:
        double Activate(double x) const override
        {
            return 1.0 / (1.0 + exp(-x));
        }

        double Derivative(double x) const override
        {
            double sigmoid = Activate(x);
            return sigmoid * (1.0 - sigmoid);
        }
    };

    class TanhActivation : public ActivationFunction
    {
    public:
        double Activate(double x) const override
        {
            return tanh(x);
        }

        double Derivative(double x) const override
        {
            return 1.0 - tanh(x) * tanh(x);
        }
    };

    class ReLUActivation : public ActivationFunction
    {
    public:
        double Activate(double x) const override
        {
            return x > 0 ? x : 0;
        }

        double Derivative(double x) const override
        {
            return x > 0 ? 1.0 : 0.0;
        }
    };

    class LinearActivation : public ActivationFunction
    {
    public:
        double Activate(double x) const override
        {
            return x;
        }

        double Derivative(double x) const override
        {
            return 1.0;
        }
    };

    std::unique_ptr<ActivationFunction> ActivationFunction::CreateActivationFunction(const ActivationFunctionType type)
    {
        switch (type)
        {
        case ActivationFunctionType::Sigmoid:
            return std::make_unique<SigmoidActivation>();
        case ActivationFunctionType::Tanh:
            return std::make_unique<TanhActivation>();
        case ActivationFunctionType::ReLU:
            return std::make_unique<ReLUActivation>();
        case ActivationFunctionType::Linear:
            return std::make_unique<LinearActivation>();
        default:
            throw std::invalid_argument("Unknown activation function type");
        }
    }
}