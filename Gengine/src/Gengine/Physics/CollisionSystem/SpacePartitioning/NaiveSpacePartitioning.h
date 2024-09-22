#pragma once
#include "SpacePartitioningStrategy.h"
#include <vector>

namespace Gengine {
    class NaiveSpacePartitioning : public SpacePartitioningStrategy {
    public:
        void Insert(const Collider& collider) override;
        Vector<std::pair<Collider, Collider>> FindCollisions() const override;
        void Clear() override;
        string ToString() override { return "NativeSpacePartitioning"; }
    private:
        Vector<Collider> m_colliders;
    };
}