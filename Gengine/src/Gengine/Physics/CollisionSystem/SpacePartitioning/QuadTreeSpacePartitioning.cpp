#include "ggpch.h"
#include "QuadTreeSpacePartitioning.h"
#include <sstream>

namespace Gengine
{
#include "QuadTreeSpacePartitioning.h"
#include <sstream>
#include <unordered_set>

    QuadTreeNode::QuadTreeNode(
        glm::vec2 source, 
        glm::vec2 size, 
        int capacity, 
        int level, 
        int maxLevel) : 
        m_Source(source),
        m_Size(size),
        m_capacity(capacity), 
        m_level(level), 
        m_maxLevel(maxLevel)
    {}

    QuadTreeNode::QuadTreeNode(const QuadTreeNode & other): 
        m_Source(other.m_Source), 
        m_Size(other.m_Size),
        m_capacity(other.m_capacity), 
        m_level(other.m_level),
        m_maxLevel(other.m_maxLevel), 
        m_colliders(other.m_colliders)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (other.m_children[i])
                m_children[i] = std::make_unique<QuadTreeNode>(*other.m_children[i]);
        }
    }

    void QuadTreeNode::Insert(const Collider& collider)
    {
        if (!Intersects(collider))
            return;

        if (IsLeaf() && (m_colliders.size() < m_capacity || m_level >= m_maxLevel))
        {
            m_colliders.push_back(collider);
            return;
        }

        if (IsLeaf())
        {
            Subdivide();
        }

        for (int i = 0; i < 4; ++i)
        {
            m_children[i]->Insert(collider);
        }
    }

    void QuadTreeNode::Subdivide()
    {
        glm::vec2 size = m_Size * 0.5f;
        glm::vec2 midPoint = m_Source + size;
        
        //Clockwise start bottom left
        m_children[0] = std::make_unique<QuadTreeNode>(
            m_Source, 
            size, m_capacity, m_level + 1, m_maxLevel);
        m_children[1] = std::make_unique<QuadTreeNode>(
            glm::vec2(m_Source.x, midPoint.y), 
            size, m_capacity, m_level + 1, m_maxLevel);
        m_children[2] = std::make_unique<QuadTreeNode>(
            midPoint,
            size, m_capacity, m_level + 1, m_maxLevel);
        m_children[3] = std::make_unique<QuadTreeNode>(
            glm::vec2(midPoint.x, m_Source.y),
            size, m_capacity, m_level + 1, m_maxLevel);

        for (const var& collider : m_colliders)
        {
            for (int i = 0; i < 4; ++i)
            {
                m_children[i]->Insert(collider);
            }
        }
        m_colliders.clear();
    }

    bool QuadTreeNode::IsLeaf() const
    {
        return m_children[0] == nullptr;
    }


    bool QuadTreeNode::Intersects(const Collider& collider) const
    {
        float testX = glm::clamp(collider.GetCenter().x, m_Source.x, m_Source.x + m_Size.x);
        float testY = glm::clamp(collider.GetCenter().y, m_Source.y, m_Source.y + m_Size.y);

        float distX = collider.GetCenter().x - testX;
        float distY = collider.GetCenter().y - testY;

        return (distX * distX + distY * distY) <= (collider.GetRadius() * collider.GetRadius());
    }

    void QuadTreeNode::Clear()
    {
        m_colliders.clear();
        for (int i = 0; i < 4; ++i)
        {
            if (m_children[i])
            {
                m_children[i]->Clear();
                m_children[i].reset();
            }
        }
    }

    void QuadTreeNode::Retrieve(Vector<Collider>& returnedColliders) const
    {
        returnedColliders.insert(returnedColliders.end(), m_colliders.begin(), m_colliders.end());
        if (!IsLeaf())
        {
            for (int i = 0; i < 4; ++i)
            {
                m_children[i]->Retrieve(returnedColliders);
            }
        }
    }

    Vector<QuadTreeNode*> QuadTreeNode::GetChildren() const
    {
        Vector<QuadTreeNode*> children;
        for (const var& child : m_children)
        {
            if (child)
            {
                children.push_back(child.get());
            }
        }
        return children;
    }

    QuadTreeSpacePartitioning::QuadTreeSpacePartitioning(
        glm::vec2 source, 
        glm::vec2 size, 
        int maxCollidersPerNode, 
        int maxLevel) : 
        m_maxCollidersPerNode(maxCollidersPerNode), m_maxLevel(maxLevel),
        m_root(source, size, maxCollidersPerNode, 0, maxLevel)
    {}

    void QuadTreeSpacePartitioning::Insert(const Collider& collider)
    {
        m_root.Insert(collider);
    }

    Vector<std::pair<Collider, Collider>> QuadTreeSpacePartitioning::FindCollisions() const
    {
        Vector<std::pair<Collider, Collider>> collisions;
        std::unordered_set<std::pair<int, int>, pair_hash> checkedPairs;

        Vector<QuadTreeNode> nodesToCheck;
        nodesToCheck.emplace_back(m_root);

        while (!nodesToCheck.empty())
        {
            QuadTreeNode first = nodesToCheck.PopBack();
            if (first.IsLeaf())
            {
                Vector<Collider> returnedColliders;
                first.Retrieve(returnedColliders);
                for(size_t i = 0; i < returnedColliders.size(); ++i)
                {
                    for (size_t j = i; j < returnedColliders.size(); ++j)
                    {
                        Collider colliderA = returnedColliders[i];
                        Collider colliderB = returnedColliders[j];

                        int idA = colliderA.GetID();
                        int idB = colliderB.GetID();

                        if (idA == idB)
                            continue;

                        std::pair<int, int> colliderPair = (idA < idB) ? std::make_pair(idA, idB) : std::make_pair(idB, idA);

                        if (checkedPairs.find(colliderPair) != checkedPairs.end())
                            continue;

                        IncrementCollisionCount();
                        if (colliderA.CollidesWith(colliderB))
                        {
                            collisions.emplace_back(colliderA, colliderB);
                        }
                        checkedPairs.insert(colliderPair);
                    }
                }
            }
            else
                for (QuadTreeNode* child : first.GetChildren())
                    nodesToCheck.emplace_back(*child);
        }
        return collisions;
    }

    void QuadTreeSpacePartitioning::Clear()
    {
        m_root.Clear();
    }

    std::string QuadTreeSpacePartitioning::ToString()
    {
        std::ostringstream oss;
        oss << "QuadTreeSpacePartitioning: MaxCollidersPerNode: " << m_maxCollidersPerNode;
        return oss.str();
    }
}
