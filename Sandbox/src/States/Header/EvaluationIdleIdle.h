#pragma once

#include "Gengine.h"

class EvaluationIdleIdle : public Gengine::State {
public:
    EvaluationIdleIdle(std::shared_ptr<Gengine::State> parent)
        : Gengine::State(parent) {
    }
    void Update(Gengine::Timestep deltaTime) override {

    }

    void ImGuiRender() override {

    }

};

