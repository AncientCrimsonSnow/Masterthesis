#include "States/Header/EvaluationIdle.h"
#include "States/Header/EvaluationProcess.h"
#include "States/Header/Main.h"
#include "States/Header/TrainingIdle.h"
#include "Config.h"

EvaluationIdle::EvaluationIdle(std::shared_ptr<Gengine::State> parent) : 
    Gengine::State(parent),
    unique_id_counter(0)
{
    m_StringInputs.push_back("");
    m_InputFieldIds.push_back(unique_id_counter++);
}

void EvaluationIdle::Init()
{
    var onEvaluationBtnPressEvent = std::make_shared<Gengine::EventCondition>(m_EvaluateBtnPress);
    var onMainMenuBtnPressEvent = std::make_shared<Gengine::EventCondition>(m_MainMenuBtnPress);
    var onTrainingBtnPressEvent = std::make_shared<Gengine::EventCondition>(m_TrainingBtnPress);

    std::shared_ptr<EvaluationProcess> evaluationProcess;
    std::shared_ptr<Main> mainMenu;
    std::shared_ptr<TrainingIdle> TrainingIdle;

    if (m_StateMachineManager->TryGetState(evaluationProcess)
        && m_StateMachineManager->TryGetState(mainMenu)
        && m_StateMachineManager->TryGetState(TrainingIdle))
        m_StateMachineManager->AddTransitions({
    { shared_from_this(), evaluationProcess, onEvaluationBtnPressEvent },
    { shared_from_this(), mainMenu, onMainMenuBtnPressEvent },
    { shared_from_this(), TrainingIdle, onTrainingBtnPressEvent } });

    m_StateMachineManager->TryGetState(m_EvaluationState);
}

void EvaluationIdle::Update(Gengine::Timestep deltaTime) 
{
    if (m_StringInputs.size() > 1)
    {
        for (size_t i = m_StringInputs.size() - 1; i-- > 0;)
        {
            if (m_StringInputs[i].empty())
            {
                m_StringInputs.erase(m_StringInputs.begin() + i);
                m_InputFieldIds.erase(m_InputFieldIds.begin() + i);
            }
        }
    }

    if (m_StringInputs.empty() || !m_StringInputs.back().empty())
    {
        m_StringInputs.push_back("");
        m_InputFieldIds.push_back(unique_id_counter++);
    }
}

void EvaluationIdle::ImGuiRender() 
{
    ImGui::Begin("Evaluation");

    ImGuiRenderInputData();
    //ImGuiRenderTextFields();

    if (ImGui::Button("Start Evaluation"))
        OnEvaluationBtnPress();
    if (ImGui::Button("Main Menu"))
        m_MainMenuBtnPress.Invoke();
    if (ImGui::Button("Training"))
        m_TrainingBtnPress.Invoke();

    ImGui::End();
}

void EvaluationIdle::ImGuiRenderTextFields()
{
    for (size_t i = 0; i < m_StringInputs.size(); ++i)
    {
        char buffer[256];
        std::strncpy(buffer, m_StringInputs[i].c_str(), sizeof(buffer));
        buffer[sizeof(buffer) - 1] = 0;

        ImGui::PushID(m_InputFieldIds[i]);

        string label = "AI Path " + std::to_string(i + 1);

        if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer)))
            m_StringInputs[i] = string(buffer);

        ImGui::PopID();
    }
}

void EvaluationIdle::ImGuiRenderInputData()
{
    int end = Config::ColliderAmountMax;

    ImGui::Text("Collider Iteration Range:");
    ImGui::InputInt("ColliderAmountMax", &end);

    if (end < 0)
        end = 0;

    Config::ColliderAmountMax = end;
}

void EvaluationIdle::OnEvaluationBtnPress() 
{
    m_EvaluateBtnPress.Invoke();
}