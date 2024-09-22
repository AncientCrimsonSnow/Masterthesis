#include "ggpch.h"
#include "NaiveSpacePartitioning.h"

namespace Gengine {
    void NaiveSpacePartitioning::Insert(const Collider& collider) {
        m_colliders.push_back(collider);
    }

    Vector<std::pair<Collider, Collider>> NaiveSpacePartitioning::FindCollisions() const {
        Vector<std::pair<Collider, Collider>> collisions;
        for (size_t i = 0; i < m_colliders.size(); ++i) {
            for (size_t j = i + 1; j < m_colliders.size(); ++j) {
                IncrementCollisionCount();
                if (m_colliders[i].CollidesWith(m_colliders[j])) {
                    collisions.emplace_back(m_colliders[i], m_colliders[j]);
                }
            }
        }
        return collisions;
    }

    void NaiveSpacePartitioning::Clear() {
        m_colliders.clear();
    }
}

