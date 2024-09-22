#pragma once

#include "Gengine.h"

class TrainingSave : public Gengine::State {
public:
    TrainingSave(std::shared_ptr<Gengine::State> parent)
        : Gengine::State(parent) {
    }

    void Init() override;
    void ImGuiRender() override;

    void Update(Gengine::Timestep deltaTime) override {

    }
private:
    Gengine::Action<void> m_OnSaveBtnPress;
    Gengine::Action<void> m_OnCancelBtnPress;

};

