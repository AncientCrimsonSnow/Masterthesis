#pragma once
#include "SpacePartitioningStrategy.h"
#include <vector>
#include <memory>
#include <utility>
#include <glm/vec2.hpp>

namespace Gengine
{
    struct pair_hash
    {
        template <class T1, class T2>
        std::size_t operator() (const std::pair<T1, T2>& pair) const
        {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };

    class QuadTreeNode
    {
    public:
        QuadTreeNode() = default;
        QuadTreeNode(glm::vec2 source, glm::vec2 size, int capacity, int level = 0, int maxLevel = 5);
        QuadTreeNode(const QuadTreeNode& other);
        void Insert(const Collider& collider);
        void Clear();
        bool IsLeaf() const;
        void Retrieve(Vector<Collider>& returnedColliders) const;
        Vector<QuadTreeNode*> GetChildren() const;
    private:
        void Subdivide();
        bool Intersects(const Collider& collider) const;

        glm::vec2 m_Source;
        glm::vec2 m_Size;
        int m_capacity;
        int m_level;
        int m_maxLevel;
        Vector<Collider> m_colliders;
        std::unique_ptr<QuadTreeNode> m_children[4]; // 0: NW, 1: NE, 2: SW, 3: SE
    };

    class QuadTreeSpacePartitioning : public SpacePartitioningStrategy
    {
    public:
        QuadTreeSpacePartitioning(
            glm::vec2 source,
            glm::vec2 size,
            int maxCollidersPerNode,
            int maxLevel = 5);
        void Insert(const Collider& collider) override;
        Vector<std::pair<Collider, Collider>> FindCollisions() const override;
        void Clear() override;
        std::string ToString() override;

    private:
        QuadTreeNode m_root;
        int m_maxCollidersPerNode;
        int m_maxLevel;
    };
}
