#pragma once
#include <utility>
#include "Gengine/Physics/CollisionSystem/Collider.h"
#include "Gengine/Utils/Vector.h"

namespace Gengine {
    class SpacePartitioningStrategy {
    public:
        virtual ~SpacePartitioningStrategy() = default;
        virtual void Insert(const Collider& collider) = 0;
        virtual Vector<std::pair<Collider, Collider>> FindCollisions() const = 0;
        virtual void Clear() = 0;
        virtual string ToString() = 0;

        void ResetCollisionCount() {
            m_collisionInvokeCheckCount = 0;
        }

        int GetCollisionInvokeCheckCount() const {
            return m_collisionInvokeCheckCount;
        }

    protected:
        void IncrementCollisionCount() const {
            ++m_collisionInvokeCheckCount;
        }

    private:
        mutable int m_collisionInvokeCheckCount = 0;
    };
}