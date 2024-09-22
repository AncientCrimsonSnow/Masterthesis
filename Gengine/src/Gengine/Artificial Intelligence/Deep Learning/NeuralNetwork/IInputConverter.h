#pragma once
#include "Gengine/Utils/Vector.h"

namespace Gengine
{
	template <typename TInput>
	class IInputConverter
	{
	public:
		virtual Vector<double> Convert(const TInput& input) const = 0;
	};
}