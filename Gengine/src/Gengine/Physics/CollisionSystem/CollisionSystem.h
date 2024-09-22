#pragma once
#include "Gengine/Physics/CollisionSystem/SpacePartitioning/SpacePartitioningStrategy.h"

namespace Gengine {
    class CollisionSystem {
    public:
        explicit CollisionSystem(SpacePartitioningStrategy* strategy);
        void AddCollider(const Collider& collider);
        Vector<std::pair<Collider, Collider>> GetCollisions() const;
        void Clear();

        void ResetCollisionCount();
        int GetCollisionInvokeCheckCount();

    private:
        SpacePartitioningStrategy* partitioningStrategy;
    };
}