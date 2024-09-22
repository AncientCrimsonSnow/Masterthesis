#pragma once

#include "Gengine.h"
#include <glm/glm.hpp>
#include "Evaluation.h"

class EvaluationIdleGraph : public Gengine::State {
public:
    EvaluationIdleGraph(std::shared_ptr<Gengine::State> parent);
    
    void Init() override;
    void Enter() override;
    void Update(Gengine::Timestep deltaTime) override;
    void ImGuiRender() override;

private:
    void DrawGraph(
        const std::unordered_map<std::string, Gengine::Vector<EvaluationResult>>& graphData,
        const std::string& xAxisName,
        const std::string& yAxisName);

    void InitGraphics();
    void DrawColliderTileMap(Gengine::Vector<Gengine::Collider> collider) const;
    void DrawAiOutputMap() const;

    static Gengine::Vector<glm::vec3> GenerateColors(int colors);
    static glm::vec3 HslToRgb(float h, float s, float l);
    std::shared_ptr<Evaluation> m_EvaluationState;

    std::shared_ptr<Gengine::Shader> m_Shader;
    std::shared_ptr<Gengine::VertexArray> m_VertexArray;

    Gengine::OrthographicCamera m_Camera;
};

