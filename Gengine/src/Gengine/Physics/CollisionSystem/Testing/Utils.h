#pragma once

#include "Gengine/Utils/Vector.h"
#include "Gengine/Physics/CollisionSystem/Collider.h"

namespace Gengine {
	class Utils {
	public:
		static Vector<Gengine::Collider> GenerateRandomColliders(
			int numColliders,
			float posMinX,
			float posMinY, 
			float posMaxX, 
			float posMaxY, 
			float radiusMin,
			float radiusMax);
	};
}