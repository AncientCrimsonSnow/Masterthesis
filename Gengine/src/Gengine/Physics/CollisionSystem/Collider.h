#pragma once
#include <glm/glm.hpp>

namespace Gengine {
    class Collider {
    public:
        Collider() = default;
        Collider(float x, float y, float radius);
        Collider(const Collider& other);
        float GetX() const;
        float GetY() const;
        float GetRadius() const;
        glm::vec2 GetCenter() const;
        bool CollidesWith(const Collider& other) const;
        int GetID() const;
    private:
        float x, y, radius;
        int id;
        static int nextID;
    };
}
