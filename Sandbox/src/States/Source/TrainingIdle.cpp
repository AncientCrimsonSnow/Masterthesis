#include "States/Header/TrainingIdle.h"
#include "States/Header/EvaluationIdle.h"
#include "States/Header/Main.h"
#include "States/Header/TrainingProcess.h"

void TrainingIdle::Init() {
    var onTrainingStartBtnEvent = std::make_shared<Gengine::EventCondition>(m_OnTrainingStartBtnPress);
    var onMainMenuBtnEvent = std::make_shared<Gengine::EventCondition>(m_OnMainMenuStateBtnPress);
    var onEvaluationBtnEvent = std::make_shared<Gengine::EventCondition>(m_OnEvaluationStateBtnPress);

    std::shared_ptr<TrainingProcess> trainingProcessState;
    std::shared_ptr<Main> mainState;
    std::shared_ptr<EvaluationIdle> evaluationIdleState;

    if (m_StateMachineManager->TryGetState(trainingProcessState)
        && m_StateMachineManager->TryGetState(mainState)
        && m_StateMachineManager->TryGetState(evaluationIdleState))
        m_StateMachineManager->AddTransitions({
            { shared_from_this(), trainingProcessState, onTrainingStartBtnEvent},
            { shared_from_this(), mainState, onMainMenuBtnEvent },
            { shared_from_this(), evaluationIdleState, onEvaluationBtnEvent } });
}

void TrainingIdle::ImGuiRender() {
    ImGui::Begin("Training");

    if (ImGui::Button("Start Training"))
        m_OnTrainingStartBtnPress.Invoke();

    if (ImGui::Button("Main Menu"))
        m_OnMainMenuStateBtnPress.Invoke();

    if (ImGui::Button("Evaluation Menu"))
        m_OnEvaluationStateBtnPress.Invoke();

    ImGui::End();
}
