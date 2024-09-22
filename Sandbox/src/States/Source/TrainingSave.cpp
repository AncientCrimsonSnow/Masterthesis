#include "States/Header/TrainingSave.h"
#include "States/Header/TrainingIdle.h"

void TrainingSave::Init() {
	var onSaveBtnPressEvent = std::make_shared<Gengine::EventCondition>(m_OnSaveBtnPress);
	var onCancelBtnPressEvent = std::make_shared<Gengine::EventCondition>(m_OnCancelBtnPress);

	std::shared_ptr<TrainingIdle> trainingIdleState;

	if (m_StateMachineManager->TryGetState(trainingIdleState))
		m_StateMachineManager->AddTransitions({
			{ shared_from_this(), trainingIdleState, onSaveBtnPressEvent },
			{ shared_from_this(), trainingIdleState, onCancelBtnPressEvent } });
}

void TrainingSave::ImGuiRender() {
	ImGui::Begin("Training");

	if (ImGui::Button("Save"))
		m_OnSaveBtnPress.Invoke();

	if (ImGui::Button("Cancel"))
		m_OnCancelBtnPress.Invoke();

	ImGui::End();
}
