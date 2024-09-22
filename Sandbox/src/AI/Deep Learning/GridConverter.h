#pragma once

#include "Gengine/Artificial Intelligence/Deep Learning/NeuralNetwork/IInputConverter.h"
#include "Gengine/Utils/Grid.h"
#include "Gengine/Physics/CollisionSystem/Collider.h"

class GridConverter : public Gengine::IInputConverter<Gengine::ArrayGrid<Gengine::Collider>>
{
	Gengine::Vector<double> Convert(const Gengine::ArrayGrid<Gengine::Collider>& input) const override;
};

