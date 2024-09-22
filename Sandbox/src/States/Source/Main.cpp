#include "States/Header/Main.h"

void Main::Init()
{
    var onTrainingBtnEvent = std::make_shared<Gengine::EventCondition>(m_OnTrainingStateBtn);
    var onEvaluationBtnEvent = std::make_shared<Gengine::EventCondition>(m_OnEvaluationStateBtn);

    std::shared_ptr<TrainingIdle> trainingIdleState;
    std::shared_ptr<EvaluationIdle> evaluationIdleState;
    if (m_StateMachineManager->TryGetState(trainingIdleState)
        && m_StateMachineManager->TryGetState(evaluationIdleState))
        m_StateMachineManager->AddTransitions({
            { shared_from_this(), trainingIdleState, onTrainingBtnEvent},
            { shared_from_this(), evaluationIdleState, onEvaluationBtnEvent } });
}