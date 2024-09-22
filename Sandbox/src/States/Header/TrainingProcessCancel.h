#pragma once

#include "Gengine.h"

class TrainingProcessCancel : public Gengine::State {
public:
    TrainingProcessCancel(std::shared_ptr<Gengine::State> parent)
        : Gengine::State(parent) {
    }

    void Init() override;

    void Enter() override {
        GG_INFO("Enter {0}-State", ToString());
        m_OnAfterCancelLogic.Invoke();
    }

    void Update(Gengine::Timestep deltaTime) override {

    }

    void ImGuiRender() override {

    }

private:
    Gengine::Action<void> m_OnAfterCancelLogic;
};

