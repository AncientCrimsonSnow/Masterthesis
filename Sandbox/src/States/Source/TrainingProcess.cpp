#include "States/Header/TrainingProcess.h"

#include "States/Header/TrainingSave.h"
#include "States/Header/TrainingProcessCancel.h"
#include "Config.h"
#include "AI/Deep Learning/NeuralNetworkPartitioningStrategy.h"

#include "Gengine/Artificial Intelligence/Deep Learning/NeuralNetwork/NoneConverter.h"
#include "AI/Deep Learning/GridConverter.h"
#include "Gengine/Physics/CollisionSystem/SpacePartitioning/GridSpacePartitioning.h"

TrainingProcess::TrainingProcess(std::shared_ptr<Gengine::State> parent) :
    Gengine::State(parent)
{}

void TrainingProcess::Init()
{
    var onTrainingFinishEvent = std::make_shared<Gengine::EventCondition>(m_OnTrainingProcessFinish);
    var onTrainingCancelBtnEvent = std::make_shared<Gengine::EventCondition>(m_OnCancelBtnPress);

    std::shared_ptr<TrainingSave> trainingSaveState;
    std::shared_ptr<TrainingProcessCancel> TrainingCancelState;

    if (m_StateMachineManager->TryGetState(trainingSaveState)
        && m_StateMachineManager->TryGetState(TrainingCancelState))
        m_StateMachineManager->AddTransitions({
            { shared_from_this(), trainingSaveState, onTrainingFinishEvent },
            { shared_from_this(), TrainingCancelState, onTrainingCancelBtnEvent } });
}

void TrainingProcess::Enter()
{           
    StartTrainingThread();
}

void TrainingProcess::Exit()
{
    m_Trainer.StopTraining();
}

void TrainingProcess::StartTraining()
{
    m_Trainer = Gengine::NeuralNetworkTrainer<Gengine::ArrayGrid<Gengine::Collider>>(
        GetLayerSize(),
        Gengine::ActivationFunctionType::Sigmoid,
        Gengine::ActivationFunctionType::Linear,
        std::make_unique<GridConverter>());

    if (m_Trainer.TryConfigureAndTrain(
        1000000,
        0.1,
        GG_BIND_EVENT_FN(TrainingProcess::CalculateReward),
        GG_BIND_EVENT_FN(TrainingProcess::GenerateTrainingsData),
        200))
        m_OnTrainingProcessFinish.Invoke();
    else
        m_OnCancelBtnPress.Invoke();
}

void TrainingProcess::StartTrainingThread()
{
    m_isInTraining = true;
    m_TrainingThread = std::thread(&TrainingProcess::StartTraining, this);
    m_TrainingThread.detach();
}

void TrainingProcess::Update(Gengine::Timestep deltaTime)
{

}

void TrainingProcess::ImGuiRender()
{
    ImGui::Begin("Training");

    ImGui::Text("Training...");

    if (ImGui::Button("Cancel"))
        m_OnCancelBtnPress.Invoke();

    ImGui::End();
}

const Gengine::Vector<int> TrainingProcess::GetLayerSize() const
{
    
    return {
        Config::DefaultGridDim.x * Config::DefaultGridDim.y,
        Config::DefaultGridDim.x * Config::DefaultGridDim.y,
        Config::DefaultGridDim.x * Config::DefaultGridDim.y,
        Config::DefaultGridDim.x * Config::DefaultGridDim.y,
        Config::DefaultGridDim.x * Config::DefaultGridDim.y};
}

double TrainingProcess::CalculateReward(const Gengine::ArrayGrid<Gengine::Collider>& input, const Gengine::Vector<double>& output)
{ 
    //Convert to Grid
    Gengine::Vector<int> normalizeArray = NeuralNetworkPartitioningStrategy::NormalizeToIntArray(
        output,
        Config::AiGridGroupsCount - 1);

    Gengine::Grid<int> cellGroupsGrid = NeuralNetworkPartitioningStrategy::NormalizedIntArrayToGrid(
        normalizeArray,
        Config::DefaultGridDim.x,
        Config::DefaultGridDim.y,
        Config::CellSize);

    //Get Collider count in group min and max and get the diff
    Gengine::Vector<Gengine::Vector<Gengine::Collider>> colliderGroups = NeuralNetworkPartitioningStrategy::GetColliderGroups(
        input,
        cellGroupsGrid,
        Config::DefaultGridDim.x,
        Config::DefaultGridDim.y,
        Config::AiGridGroupsCount);

    size_t minLength = std::numeric_limits<size_t>::max();
    size_t maxLength = 0;

    for (const var& group : colliderGroups)
    {
        size_t length = group.size();
        if (length < minLength)
        {
            minLength = length;
        }
        if (length > maxLength)
        {
            maxLength = length;
        }
    }

    //1. important Parameter:
    int colliderDiff = maxLength - minLength;

    //Calculation of the collision Checkcount.

    //2. important Parameter (only important if collisionErrorValue is 0):
    int collisionCheckCount = 0;


    Gengine::Vector<std::pair<int, int>> groupNeighbourPairs = NeuralNetworkPartitioningStrategy::GetNeighbourGroups(cellGroupsGrid);

    Gengine::Vector<std::pair<Gengine::Collider, Gengine::Collider>> collisions;

    for (var& colliderGroup : colliderGroups)
    {
        for (size_t i = 0; i < colliderGroup.size(); ++i)
        {
            for (size_t j = i + 1; j < colliderGroup.size(); ++j)
            {
                collisionCheckCount++;
                if (colliderGroup[i].CollidesWith(colliderGroup[j]))
                    collisions.emplace_back(colliderGroup[i], colliderGroup[j]);
            }
        }
    }

    for (var& pair : groupNeighbourPairs)
    {
        var& colliderGroupOne = colliderGroups[pair.first];
        var& colliderGroupTwo = colliderGroups[pair.second];

        for (var& colliderOne : colliderGroupOne)
        {
            for (var& colliderTwo : colliderGroupTwo)
            {
                collisionCheckCount++;
                if (colliderOne.CollidesWith(colliderTwo))
                {
                    collisions.emplace_back(colliderOne, colliderTwo);
                }
            }
        }
    }

    var gridSpacePartitioning = Gengine::GridSpacePartitioning(Config::CellSize);
    for (var& cell : input)
        for (var& collider : cell)
            gridSpacePartitioning.Insert(collider);

    int trueCollisionsCount = gridSpacePartitioning.FindCollisions().size();

    //3. important Parameter:
    int collisionErrorValue = abs(trueCollisionsCount - static_cast<int>(collisions.size()));


    int neighboursWithSameIdCount = 0;
    for (int y = 0; y < Config::DefaultGridDim.y; y++)
    {
        for (int x = 0; x < Config::DefaultGridDim.x; x++)
        {
            int groupId;
            
            if (cellGroupsGrid.TryGetCell({ x, y }, groupId))
            {
                var neighbours = cellGroupsGrid.GetNeighborKeys({ x, y });
                for (var& neighbour : neighbours)
                {
                    int neighbourGroupId;
                    if (cellGroupsGrid.TryGetCell(neighbour, neighbourGroupId))
                        if (groupId == neighbourGroupId)
                            neighboursWithSameIdCount++;
                }
            }

        }
    }

    //4. important Parameter;
    double neighboursWithSameIdCountAverage = static_cast<double>(neighboursWithSameIdCount) / (Config::DefaultGridDim.y * Config::DefaultGridDim.x);
    return RewardWeights(
        neighboursWithSameIdCountAverage,
        collisionErrorValue,
        colliderDiff,
        collisionCheckCount,
        trueCollisionsCount);
}

Gengine::ArrayGrid<Gengine::Collider> TrainingProcess::GenerateTrainingsData()
{
    Gengine::Random random;
    
    int colliderCount = random.NextInt(Config::TrainingColliderRange.x, Config::TrainingColliderRange.y);

    var colliders = Gengine::Utils::GenerateRandomColliders(
        colliderCount,
        Config::GridInputPos.x,
        Config::GridInputPos.y,
        Config::GridInputPos.x + Config::GridSize.x,
        Config::GridInputPos.y + Config::GridSize.y,
        Config::RadiusRange.x,
        Config::RadiusRange.y);

    Gengine::ArrayGrid<Gengine::Collider> grid(Config::CellSize);

    for (const var& collider : colliders)
        grid.Insert(collider, collider.GetX(), collider.GetY());

    return grid;
}

double TrainingProcess::RewardWeights(
    double neighboursWithSameIdCountAverage, 
    double collisionErrorValue, 
    double colliderDiff, 
    double collisionCheckCount,
    double collisionCheckCountBase)
{
    const double neighboursWithSameIdCountAverageTarget = 3;
    const double collisionErrorValueTarget = 0.0;
    const double colliderDiffTarget = 1; // Nahe 0

    double reward = 0.0;
    double stage1Reward = 0.0;
    double stage2Reward = 0.0;
    double stage3Reward = 0.0;
    double stage4Reward = 0.0;

    // 1. Stufe: Reduziere colliderDiff auf colliderDiffTarget
    if (colliderDiff > colliderDiffTarget)
        stage1Reward = 
        10.0 * ReziprokeFunction(colliderDiff, colliderDiffTarget, 0.25f) * 
        10.0 * ReziprokeFunction(colliderDiff, colliderDiffTarget, 0.25f) * 
        10.0 * ReziprokeFunction(colliderDiff, colliderDiffTarget, 0.25f);
    else
    {
        stage1Reward = 10.0 * 10.0 * 10.0;

        // 2. Stufe: Erhögung von neighboursWithSameIdCountAverage auf neighboursWithSameIdCountAverageTarget
        if (neighboursWithSameIdCountAverage < neighboursWithSameIdCountAverageTarget)
            stage2Reward = 
            10.0 * QuadraticNormalization(neighboursWithSameIdCountAverage, neighboursWithSameIdCountAverageTarget) *
            10.0 * QuadraticNormalization(neighboursWithSameIdCountAverage, neighboursWithSameIdCountAverageTarget) *
            10.0 * QuadraticNormalization(neighboursWithSameIdCountAverage, neighboursWithSameIdCountAverageTarget);
        else
        {
            stage2Reward = 10.0 * 10.0 * 10.0;

            // 3. Stufe: Reduziere collisionErrorValue auf collisionErrorValueTarget
            if (collisionErrorValue != 0)
                stage3Reward = 
                10.0 * ReziprokeFunction(collisionErrorValue, collisionErrorValueTarget, 0.25f) *
                10.0 * ReziprokeFunction(collisionErrorValue, collisionErrorValueTarget, 0.25f) *
                10.0 * ReziprokeFunction(collisionErrorValue, collisionErrorValueTarget, 0.25f);
            else
            {
                stage3Reward = 10.0 * 10.0 * 10.0;

                // 4. Stufe: Reduziere collisionCheckCount so weit es geht. 
                //Unterschied
                int collisionCheckCountDelta = collisionCheckCountBase - collisionCheckCount;
                //Reduzierung des Unterschiedes 
                if (collisionCheckCountDelta > 0)
                    stage4Reward = 
                    10.0 * ReziprokeFunction(collisionCheckCountDelta, 0, 0.25f) *
                    10.0 * ReziprokeFunction(collisionCheckCountDelta, 0, 0.25f) *
                    10.0 * ReziprokeFunction(collisionCheckCountDelta, 0, 0.25f);
                else
                    stage4Reward = 
                    10.0 * -collisionCheckCountDelta *
                    10.0 * -collisionCheckCountDelta *
                    10.0 * -collisionCheckCountDelta;
            }
        }
    }

    // Der Gesamtreward setzt sich aus den jeweiligen Stufen zusammen
    reward = stage1Reward + stage2Reward + stage3Reward + stage4Reward;

    return reward;
}
