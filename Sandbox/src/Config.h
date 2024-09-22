#pragma once

#include <glm/glm.hpp>

class Config
{
public:
    // TODO Move to GUI

    static constexpr glm::vec2 CameraDim = glm::vec2(2048.0f, 1024.0f);
    static constexpr glm::vec2 CameraPos = glm::vec2(1024.0f, 512.0f);

    static constexpr glm::vec2 GridSize = glm::vec2(
        CameraDim.x / 2.f,
        CameraDim.y);
    static constexpr glm::vec2 GridInputPos = glm::vec2(
        CameraPos.x - CameraDim.x / 2.f,
        CameraPos.y - CameraDim.y / 2.f);
    static constexpr glm::vec2 GridOutputPos = glm::vec2(
        CameraDim.x / 2.f,
        CameraPos.y - CameraDim.y / 2.f);

    static constexpr glm::uvec2 DefaultGridDim = glm::uvec2(8, 8);

    //Ensure that x and y is bigger then RadiusRange.y
    static constexpr glm::vec2 CellSize = glm::vec2(
        Config::GridSize.x / Config::DefaultGridDim.x,
        Config::GridSize.y / Config::DefaultGridDim.y);

    static constexpr glm::vec2 RadiusRange = glm::vec2(16.f, 28.f);

    static constexpr glm::uvec2 TrainingColliderRange = glm::uvec2(1000, 1000);
    static constexpr int AiGridGroupsCount = 4.0f;

    static int ColliderAmountMax;
    static int ColliderSliceToRender;
};
