#pragma once

#include "Gengine.h"
#include "States/Header/Evaluation.h"
#include "Gengine/Physics/CollisionSystem/SpacePartitioning/SpacePartitioningStrategy.h"
#include "Gengine/Physics/CollisionSystem/Collider.h"


class EvaluationProcess : public Gengine::State 
{
public:
    EvaluationProcess(std::shared_ptr<Gengine::State> parent);

    void Init() override;
    void Enter() override;
    void Update(Gengine::Timestep deltaTime) override {}
    void ImGuiRender() override;
    void Exit() override;
    
private:
    std::atomic<bool> m_isProcessing;
    std::thread m_processThread;

    float m_progress;
    Gengine::Action<void> m_OnEvaluationProcessFinish;
    Gengine::Action<void> m_OnCancelBtnPress;

    std::shared_ptr<Evaluation> m_EvaluationState;

    void StartProcess();
    void ProcessLoop();
    const void Iterate(int colliderCount);
    const EvaluationResult EvaluateStrategy(
        Gengine::SpacePartitioningStrategy* strategy,
        const Gengine::Vector<Gengine::Collider>& colliders,
        int collisionsCount) const;
};