#pragma once

#include "Gengine.h"
struct EvaluationResult
{
    int collisionCheckCount;
    float runtime;
    int errorValue;
    int collisionsCount;

    float GetValueByName(const std::string& name) const
    {
        if (name == "collisionCheckCount") return static_cast<float>(collisionCheckCount);
        if (name == "runtime") return runtime;
        if (name == "errorValue") return errorValue;
        if (name == "collisionsCount") return static_cast<float>(collisionsCount);
        return -1;
    }
};

class Evaluation : public Gengine::State 
{
public:
    Evaluation(std::shared_ptr<Gengine::State> parent);
    void Update(Gengine::Timestep deltaTime) override;
    void ImGuiRender() override;

    const Gengine::Vector<Gengine::Collider>& GetCollider(int count) const;
    void SetCollider(Gengine::Vector<Gengine::Collider>&& collider);
    void SetGraphIndex(const string& key, int colliderCounter, EvaluationResult data);
    const std::unordered_map<string, Gengine::Vector<EvaluationResult>>& GetGraphData() const;

    static void SetGrid(Gengine::Grid<int> grid, int slice);
    static Gengine::Grid<int>& GetGridAt(int slice);
private:
    Gengine::Vector<Gengine::Vector<Gengine::Collider>> m_CollidersSliceMap;
    std::unordered_map<string, Gengine::Vector<EvaluationResult>> m_graphData;

    static Gengine::Vector<Gengine::Grid<int>> s_AiOutputGrids;
};