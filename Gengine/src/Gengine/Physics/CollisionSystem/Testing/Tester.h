#pragma once

#include "Gengine/Physics/CollisionSystem/SpacePartitioning/SpacePartitioningStrategy.h"

namespace Gengine {
	class Tester {
	public:
		void Run();
	private:
		void TestCollisionSystem(const std::string& name, SpacePartitioningStrategy* strategy, const Vector<Collider>& colliders);
		float CalculateDistance(const Gengine::Collider& a, const Gengine::Collider& b);
	};
}