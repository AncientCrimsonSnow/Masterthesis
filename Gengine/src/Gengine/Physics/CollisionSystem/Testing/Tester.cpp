
#include "ggpch.h"
#include "Tester.h"

#include "Gengine/Physics/CollisionSystem/Collider.h"
#include "Gengine/Physics/CollisionSystem/CollisionSystem.h"
#include "Gengine/Physics/CollisionSystem/SpacePartitioning/NaiveSpacePartitioning.h"
#include "Gengine/Physics/CollisionSystem/SpacePartitioning/GridSpacePartitioning.h"

#include "Gengine/Physics/CollisionSystem/Testing/Utils.h"

namespace Gengine {
    void Tester::Run() {
        /*
        var collider = Utils::GenerateRandomColliders(100);

        NaiveSpacePartitioning naivePartitioning;
        TestCollisionSystem("Naive", &naivePartitioning, collider);

        //Make sure: max radius * 2 >= Cellzize
        float cellSize = 10.f;

        GridSpacePartitioning gridPartitioning(cellSize);
        TestCollisionSystem("Grid", &gridPartitioning, collider);
        */
    }

    void Tester::TestCollisionSystem(const std::string& name, SpacePartitioningStrategy* strategy, const Vector<Collider>& colliders) {
        CollisionSystem collisionSystem(strategy);
        for (const var& collider : colliders) {
            collisionSystem.AddCollider(collider);
        }

        var collisions = collisionSystem.GetCollisions();
        GG_CORE_INFO("{} collisions: {}, CollisionCheckCount: {}", name, collisions.size(), collisionSystem.GetCollisionInvokeCheckCount());

        /*
        for (const var& collision : collisions) {
            const var& colliderA = collision.first;
            const var& colliderB = collision.second;
            float distance = CalculateDistance(colliderA, colliderB);
            float sumRadii = colliderA.GetRadius() + colliderB.GetRadius();

            if (sumRadii < distance) {
                GG_CORE_ERROR("Collision between: ({:.2f}, {:.2f}) [Radius: {:.2f}] and ({:.2f}, {:.2f}) [Radius: {:.2f}] - Distance: {:.2f}",
                    colliderA.GetX(), colliderA.GetY(), colliderA.GetRadius(),
                    colliderB.GetX(), colliderB.GetY(), colliderB.GetRadius(),
                    distance);
            }
            else {
                GG_CORE_INFO("Collision between: ({:.2f}, {:.2f}) [Radius: {:.2f}] and ({:.2f}, {:.2f}) [Radius: {:.2f}] - Distance: {:.2f}",
                    colliderA.GetX(), colliderA.GetY(), colliderA.GetRadius(),
                    colliderB.GetX(), colliderB.GetY(), colliderB.GetRadius(),
                    distance);
            }
        }
        */
    }

    float Tester::CalculateDistance(const Gengine::Collider& a, const Gengine::Collider& b) {
        float dx = a.GetX() - b.GetX();
        float dy = a.GetY() - b.GetY();
        return std::sqrt(dx * dx + dy * dy);
    }
}


