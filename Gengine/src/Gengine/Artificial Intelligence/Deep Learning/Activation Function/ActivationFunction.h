#pragma once
#include "ActivationFunctionType.h"

namespace Gengine
{
    class ActivationFunction
    {
    public:
        virtual ~ActivationFunction() = default;
        virtual double Activate(double x) const = 0;
        virtual double Derivative(double x) const = 0;

        static std::unique_ptr<ActivationFunction> CreateActivationFunction(const ActivationFunctionType type);
    };
}

