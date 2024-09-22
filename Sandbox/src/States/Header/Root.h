#pragma once

#include "Gengine.h"
#include "imgui/imgui.h"
#include "Gengine/Utils/Vector.h"
#include "Gengine/Utils/Action.h"
#include "Gengine/StateMachine/Conditions/EventCondition.h"

#include "Main.h";
#include "Training.h"
#include "Evaluation.h"

#include "TrainingIdle.h";
#include "TrainingProcess.h";
#include "TrainingSave.h";
#include "TrainingProcessCancel.h"

#include "EvaluationIdle.h";
#include "EvaluationProcess.h";
#include "EvaluationProcessCancel.h"

#include "EvaluationIdleIdle.h"
#include "EvaluationIdleGraph.h";


class Root : public Gengine::State {
public:
	Root();

	void Init() override;

	void Update(Gengine::Timestep deltaTime) override;
	
	void ImGuiRender() override;
};

