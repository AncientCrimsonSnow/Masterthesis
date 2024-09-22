#include "States/Header/Evaluation.h"
#include "Config.h"


Evaluation::Evaluation(std::shared_ptr<Gengine::State> parent) :
    Gengine::State(parent)
{}

void Evaluation::Update(Gengine::Timestep deltaTime)
{}

void Evaluation::ImGuiRender()
{}

const Gengine::Vector<Gengine::Collider>& Evaluation::GetCollider(int count) const
{
    if (count >= m_CollidersSliceMap.size())
    {
        GG_WARN("Collideramount of {0} is not being generated", count);
        return m_CollidersSliceMap.back();
    }
    return m_CollidersSliceMap[count];
}

void Evaluation::SetCollider(Gengine::Vector<Gengine::Collider>&& collider)
{
    m_CollidersSliceMap.resize(collider.size() + 1);
    m_CollidersSliceMap[collider.size()] = collider;
}

void Evaluation::SetGrid(Gengine::Grid<int> grid, int slice)
{
    s_AiOutputGrids.resize(slice + 1);
    s_AiOutputGrids[slice] = grid;
}

Gengine::Grid<int>& Evaluation::GetGridAt(int slice)
{
    if (slice >= s_AiOutputGrids.size())
    {
        GG_WARN("Collideramount of {0} is not being generated", slice);
        return s_AiOutputGrids.back();
    }
    return s_AiOutputGrids[slice];
}

void Evaluation::SetGraphIndex(const std::string& key, int colliderCounter, EvaluationResult data)
{
    size_t neededSize = Config::ColliderAmountMax + 2;
    if (m_graphData[key].size() < neededSize)
        m_graphData[key].resize(neededSize);

    m_graphData[key][colliderCounter] = data;
}

const std::unordered_map<string, Gengine::Vector<EvaluationResult>>& Evaluation::GetGraphData() const
{
    return m_graphData;
}

Gengine::Vector<Gengine::Grid<int>> Evaluation::s_AiOutputGrids;


