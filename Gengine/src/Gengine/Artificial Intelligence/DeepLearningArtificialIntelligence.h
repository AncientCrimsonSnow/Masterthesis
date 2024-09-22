#pragma once

#include "Gengine/Utils/Vector.h"

namespace Gengine
{
    class DeepLearningArtificialIntelligence
    {
    public:
        virtual ~DeepLearningArtificialIntelligence() = default;
        virtual Vector<double> Inference(const Vector<double>& input) = 0;
    };
}