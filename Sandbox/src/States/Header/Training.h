#pragma once

#include "Gengine.h"


class Training : public Gengine::State {
public:
    Training(std::shared_ptr<Gengine::State> parent)
        : Gengine::State(parent) {
    }

    void Update(Gengine::Timestep deltaTime) override {

    }

    void ImGuiRender() override {

    }

};

