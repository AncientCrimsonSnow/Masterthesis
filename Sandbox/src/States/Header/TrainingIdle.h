#pragma once

#include "Gengine.h"

class TrainingIdle : public Gengine::State {
public:
    TrainingIdle(std::shared_ptr<Gengine::State> parent) : 
        Gengine::State(parent) {}

    void Init() override;
    void Update(Gengine::Timestep deltaTime) override {}
    void ImGuiRender() override;
private:
    Gengine::Action<void> m_OnTrainingStartBtnPress;
    Gengine::Action<void> m_OnMainMenuStateBtnPress;
    Gengine::Action<void> m_OnEvaluationStateBtnPress;
};

