#include "States/Header/EvaluationProcess.h"
#include "States/Header/EvaluationProcessCancel.h"
#include "States/Header/EvaluationIdleGraph.h"
#include "Gengine/Physics/CollisionSystem/CollisionSystem.h"
#include "Gengine/Physics/CollisionSystem/SpacePartitioning/NaiveSpacePartitioning.h"
#include "Gengine/Physics/CollisionSystem/SpacePartitioning/GridSpacePartitioning.h"
#include "Gengine/Physics/CollisionSystem/SpacePartitioning/QuadTreeSpacePartitioning.h"
#include "Config.h"
#include "Gengine/Artificial Intelligence/Deep Learning/NeuralNetwork/NeuralNetwork.h"
#include "AI/Deep Learning/NeuralNetworkPartitioningStrategy.h"

EvaluationProcess::EvaluationProcess(std::shared_ptr<Gengine::State> parent) :
    Gengine::State(parent),
    m_progress(0){}

void EvaluationProcess::Init() 
{
    var onEvaluationFinishEvent = std::make_shared<Gengine::EventCondition>(m_OnEvaluationProcessFinish);
    var onEvaluationCancelBtnEvent = std::make_shared<Gengine::EventCondition>(m_OnCancelBtnPress);

    std::shared_ptr<EvaluationIdleGraph> evaluationIdleState;
    std::shared_ptr<EvaluationProcessCancel> evaluationCancelState;

    if (m_StateMachineManager->TryGetState(evaluationIdleState)
        && m_StateMachineManager->TryGetState(evaluationCancelState))
        m_StateMachineManager->AddTransitions({
            { shared_from_this(), evaluationIdleState, onEvaluationFinishEvent },
            { shared_from_this(), evaluationCancelState, onEvaluationCancelBtnEvent } });

    m_StateMachineManager->TryGetState(m_EvaluationState);
}

void EvaluationProcess::Enter() 
{
    StartProcess();
}

void EvaluationProcess::StartProcess()
{
    m_progress = 0;

    m_isProcessing = true;
    m_processThread = std::thread(&EvaluationProcess::ProcessLoop, this);
    m_processThread.detach();
}

void EvaluationProcess::ProcessLoop()
{
    for (int i = 0; i++ <= Config::ColliderAmountMax;)
    {
        if (!m_isProcessing)
            return;
        m_progress = (static_cast<float>(i)) / Config::ColliderAmountMax;
        Iterate(i);
    }
    m_isProcessing = false;
    m_OnEvaluationProcessFinish.Invoke();
}

const void EvaluationProcess::Iterate(int colliderCount)
{
    m_EvaluationState->SetCollider(
        Gengine::Utils::GenerateRandomColliders(
            colliderCount,
            Config::GridInputPos.x,
            Config::GridInputPos.y,
            Config::GridInputPos.x + Config::GridSize.x,
            Config::GridInputPos.y + Config::GridSize.y,
            Config::RadiusRange.x, 
            Config::RadiusRange.y));
    
    var gridSpacePartitioning = std::make_shared<Gengine::GridSpacePartitioning>(glm::vec2(64.f));
    Gengine::NeuralNetworkData neuralNetworkData;
    
    const Gengine::Vector<Gengine::Collider>& collider = m_EvaluationState->GetCollider(colliderCount);
    EvaluationResult sourceCollisionCountResult = EvaluateStrategy(
        gridSpacePartitioning.get(),
        collider, -1);
    int collisionCount = sourceCollisionCountResult.collisionsCount;

    if (neuralNetworkData.TryLoad("ModelData", neuralNetworkData))
    {
        var neuralNetworkPtr = std::make_unique<Gengine::NeuralNetwork>(neuralNetworkData);
        var neuralNetworkStrategy = std::make_shared<NeuralNetworkPartitioningStrategy>(std::move(neuralNetworkPtr), Config::CellSize);

        EvaluationResult result = EvaluateStrategy(
            neuralNetworkStrategy.get(),
            collider,
            collisionCount);

        neuralNetworkStrategy->Clear();

        m_EvaluationState->SetGraphIndex(
            neuralNetworkStrategy->ToString(),
            colliderCount,
            result);
    }
    else
        GG_WARN("Cant Load Neural NetworkData");

    Gengine::Vector<std::shared_ptr<Gengine::SpacePartitioningStrategy>> strategies = {
        std::make_shared<Gengine::GridSpacePartitioning>(glm::vec2(64.f)),
        std::make_shared<Gengine::GridSpacePartitioning>(glm::vec2(128.f)),
        std::make_shared<Gengine::GridSpacePartitioning>(glm::vec2(256.f)),
        std::make_shared<Gengine::GridSpacePartitioning>(glm::vec2(512.f)),
        std::make_shared<Gengine::QuadTreeSpacePartitioning>(
            Config::GridInputPos,
            Config::GridSize,
            100
        )
    };

  
    for (int i = 0; i < strategies.size(); ++i)
    {
        EvaluationResult result = EvaluateStrategy(
            strategies[i].get(),
            collider,
            collisionCount);

        strategies[i]->Clear();

        m_EvaluationState->SetGraphIndex(
            strategies[i]->ToString(),
            colliderCount,
            result);

    }
}

void EvaluationProcess::ImGuiRender() 
{
    ImGui::Begin("Evaluation");

    ImGui::Text("Loading...");
    ImGui::ProgressBar(m_progress, ImVec2(0.0f, 0.0f));

    if (ImGui::Button("Cancel"))
        m_OnCancelBtnPress.Invoke();

    ImGui::End();
}

void EvaluationProcess::Exit()
{
    m_isProcessing = false;
}

const EvaluationResult EvaluationProcess::EvaluateStrategy(
    Gengine::SpacePartitioningStrategy* strategy, 
    const Gengine::Vector<Gengine::Collider>& colliders,
    int collisionsCount) const 
{
    float timeMeasure = Gengine::Time::GetRealtimeSinceStart();

    Gengine::CollisionSystem collisionSystem(strategy);

    for (const var& collider : colliders)
        collisionSystem.AddCollider(collider);

    var collisions = collisionSystem.GetCollisions();

    timeMeasure = Gengine::Time::GetRealtimeSinceStart() - timeMeasure;

    EvaluationResult result{};
    result.collisionCheckCount = collisionSystem.GetCollisionInvokeCheckCount();
    result.runtime = timeMeasure;
    result.errorValue = static_cast<int>(collisions.size()) - collisionsCount;
    result.collisionsCount = collisions.size();

    return result;
}



