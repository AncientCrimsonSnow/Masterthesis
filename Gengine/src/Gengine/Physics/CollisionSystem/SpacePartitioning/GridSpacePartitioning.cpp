#include "ggpch.h"
#include "GridSpacePartitioning.h"
#include <Gengine/Utils/Grid.h>

namespace Gengine 
{
    GridSpacePartitioning::GridSpacePartitioning(glm::vec2 cellSize) : 
        m_Grid(cellSize)
    {}

    void GridSpacePartitioning::Insert(const Collider& collider) 
    {
        m_Grid.Insert(collider, collider.GetX(), collider.GetY());
    }

    Vector<std::pair<Collider, Collider>> GridSpacePartitioning::FindCollisions() const 
    {
        Vector<std::pair<Collider, Collider>> collisions;
        for (const var& [cellKey, colliders] : m_Grid.GetCells()) 
        {
            for (size_t i = 0; i < colliders.size(); ++i) 
            {
                for (size_t j = i + 1; j < colliders.size(); ++j) 
                {
                    IncrementCollisionCount();
                    if (colliders[i].CollidesWith(colliders[j])) 
                    {
                        collisions.emplace_back(colliders[i], colliders[j]);
                    }
                }
            }

            var neighbors = m_Grid.GetNeighborKeysSingular(cellKey);
            for (const var& neighbor : neighbors) 
            {
                Vector<Collider> neighborColliders;
                if (m_Grid.TryGetCell(neighbor, neighborColliders))
                    for (const var& collider : colliders)
                    {
                        for (const var& neighborCollider : neighborColliders)
                        {
                            IncrementCollisionCount();
                            if (collider.CollidesWith(neighborCollider))
                            {
                                collisions.emplace_back(collider, neighborCollider);
                            }
                        }
                    }
            }
        }
        return collisions;
    }

    void GridSpacePartitioning::Clear() 
    {
        m_Grid.Clear();
    }
}