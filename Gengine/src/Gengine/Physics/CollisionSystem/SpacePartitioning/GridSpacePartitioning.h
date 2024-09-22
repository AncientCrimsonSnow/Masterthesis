#pragma once
#include "SpacePartitioningStrategy.h"
#include <unordered_map>
#include <utility>
#include <Gengine/Utils/Grid.h>

namespace Gengine 
{
    class GridSpacePartitioning : public SpacePartitioningStrategy 
    {
    public:
        explicit GridSpacePartitioning(glm::vec2 cellSize);
        void Insert(const Collider& collider) override;
        Vector<std::pair<Collider, Collider>> FindCollisions() const override;
        void Clear() override;
        string ToString() override
        {
            glm::vec2 cellSize = m_Grid.GetCellSize();
            std::ostringstream oss;
            oss << "GridSpacePartitioning: Size: (" << cellSize.x << "," << cellSize.y << ")";
            return oss.str();
        }

    private:
        ArrayGrid<Collider> m_Grid;
    };
}

