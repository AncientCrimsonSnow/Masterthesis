#include "ggpch.h"
#include "Collider.h"

#include <cmath>

namespace Gengine {

    int Collider::nextID = 0;

    Collider::Collider(float x, float y, float radius) :
        x(x), 
        y(y), 
        radius(radius), 
        id(nextID++) {}

    Collider::Collider(const Collider& other) :
        x(other.x),
        y(other.y),
        radius(other.radius),
        id(other.id)
    {}

    float Collider::GetX() const {
        return x;
    }

    float Collider::GetY() const {
        return y;
    }

    float Collider::GetRadius() const {
        return radius;
    }

    glm::vec2 Collider::GetCenter() const
    {
        return glm::vec2(x, y);
    }

    bool Collider::CollidesWith(const Collider& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float distanceSquared = dx * dx + dy * dy;
        float radiusSum = radius + other.radius;
        return distanceSquared < radiusSum * radiusSum;
    }

    int Collider::GetID() const
    {
        return id;
    }
}