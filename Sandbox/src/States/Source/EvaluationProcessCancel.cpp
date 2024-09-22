#include "States/Header/EvaluationProcessCancel.h"
#include "States/Header/EvaluationIdle.h"

void EvaluationProcessCancel::Init() {
	var onAfterCancelLogicEvent = std::make_shared<Gengine::EventCondition>(m_OnAfterCancelLogic);
	std::shared_ptr<EvaluationIdle> evaluationIdleState;

	if (m_StateMachineManager->TryGetState(evaluationIdleState))
		m_StateMachineManager->AddTransition(
			{ shared_from_this(), evaluationIdleState, onAfterCancelLogicEvent });
}
