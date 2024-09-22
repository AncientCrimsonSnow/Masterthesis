#include "States/Header/TrainingProcessCancel.h"
#include "States/Header/TrainingSave.h"

void TrainingProcessCancel::Init() {
	var onAfterCancelLogicEvent = std::make_shared<Gengine::EventCondition>(m_OnAfterCancelLogic);
	std::shared_ptr<TrainingSave> trainingSaveState;

	if (m_StateMachineManager->TryGetState(trainingSaveState))
		m_StateMachineManager->AddTransition(
			{ shared_from_this(), trainingSaveState, onAfterCancelLogicEvent });
}


