#pragma once

#include "Gengine.h"

#include "TrainingIdle.h"
#include "EvaluationIdle.h"

class Main : public Gengine::State {
public:
    Main(
        std::shared_ptr<Gengine::State> parent)
        : 
        Gengine::State(parent) {}

    void Init() override;

    void Update(Gengine::Timestep deltaTime) override {

    }

    void ImGuiRender() override {
        ImGui::Begin("Main Menu");

        if (ImGui::Button("Training"))
            m_OnTrainingStateBtn.Invoke();

        if (ImGui::Button("Evaluation"))
            m_OnEvaluationStateBtn.Invoke();

        ImGui::End();
    }

private:
    Gengine::Action<void> m_OnTrainingStateBtn;
    Gengine::Action<void> m_OnEvaluationStateBtn;
};

