#pragma once

#include "Gengine.h"
#include "States/Header/Evaluation.h"

class EvaluationIdle : public Gengine::State {
public:
    EvaluationIdle(std::shared_ptr<Gengine::State> parent);

    void Init() override;
    void Update(Gengine::Timestep deltaTime) override;
    void ImGuiRender() override;

    void ImGuiRenderTextFields();
    void ImGuiRenderInputData();
    void OnEvaluationBtnPress();
private:
    Gengine::Action<void> m_EvaluateBtnPress;
    Gengine::Action<void> m_MainMenuBtnPress;
    Gengine::Action<void> m_TrainingBtnPress;

    Gengine::Vector<string> m_StringInputs;
    Gengine::Vector<int> m_InputFieldIds;
    int unique_id_counter;

    std::shared_ptr<Evaluation> m_EvaluationState;
};

