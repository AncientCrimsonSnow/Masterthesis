#include "ggpch.h"

#include "CollisionSystem.h"
#include "Gengine/Utils/Vector.h"


namespace Gengine {
    CollisionSystem::CollisionSystem(SpacePartitioningStrategy* strategy) : partitioningStrategy(strategy) {}

    void CollisionSystem::AddCollider(const Collider& collider) {
        partitioningStrategy->Insert(collider);
    }

    Vector<std::pair<Collider, Collider>> CollisionSystem::GetCollisions() const {
        return partitioningStrategy->FindCollisions();
    }

    void CollisionSystem::Clear() {
        partitioningStrategy->Clear();
    }
    void CollisionSystem::ResetCollisionCount() {
        partitioningStrategy->ResetCollisionCount();
    }
    int CollisionSystem::GetCollisionInvokeCheckCount() {
        return partitioningStrategy->GetCollisionInvokeCheckCount();
    }
}