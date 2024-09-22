#include "ggpch.h"
#include "Utils.h"
#include "Gengine/Utils/Random.h"

namespace Gengine {
    Vector<Collider> Utils::GenerateRandomColliders(
        int numColliders,
        float posMinX,
        float posMinY,
        float posMaxX,
        float posMaxY,
        float radiusMin,
        float radiusMax) {
        Vector<Collider> colliders;
        colliders.reserve(numColliders);

        Random random;

        for (int i = 0; i < numColliders; ++i)
        {
            float x = random.NextFloat(posMinX, posMaxX);
            float y = random.NextFloat(posMinY, posMaxY);
            float radius = random.NextFloat(radiusMin, radiusMax);
            colliders.emplace_back(x, y, radius);
        }

        return colliders;
    }
}