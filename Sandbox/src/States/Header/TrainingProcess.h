#pragma once

#include "Gengine.h"

class TrainingProcess : public Gengine::State {
public:
    TrainingProcess(std::shared_ptr<Gengine::State> parent);

    void Init() override;
    void Enter() override;
    void Exit() override;
    void Update(Gengine::Timestep deltaTime) override;
    void ImGuiRender() override;
private:
    std::atomic<bool> m_isInTraining;
    std::thread m_TrainingThread;
    Gengine::Action<void> m_OnTrainingProcessFinish;
    Gengine::Action<void> m_OnCancelBtnPress;
    Gengine::NeuralNetworkTrainer<Gengine::ArrayGrid<Gengine::Collider>> m_Trainer;

    const Gengine::Vector<int> GetLayerSize() const;
    double CalculateReward(const Gengine::ArrayGrid<Gengine::Collider>& input, const Gengine::Vector<double>& output);
    Gengine::ArrayGrid<Gengine::Collider> GenerateTrainingsData();
    void StartTraining();
    void StartTrainingThread();
    
    static double RewardWeights(
        double neighboursWithSameIdCountAverage,
        double collisionErrorValue,
        double colliderDiff,
        double collisionCheckCount,
        double collisionCheckCountBase);

    //f( x, threshold )= ( 1 - threshold + x ) ^-1.
    //threshold gibt an, an welcher stelle 1 zurück gegeben wird, wenn x = threshold ist. 
    //x Werte unter threshold wird dann auf y gemapped, somit kann der Wert 1 nie übersteigen.
    //x Werte über threshold wird nach unendlich 0. 
    //exponent gibt an, wie schnell sich die Funktion gen 0 annährt. (bei 0 niemals, bei 1 halbiert sich der rückgabewert jeden schritt, >1 immer schneller)
    static double ReziprokeFunction(double x, double threshold, double exponent)
    {
        if (exponent < 0)
            exponent = 0;
        double result = pow(1 - threshold + x, -exponent);
        return result;
    }

    //f( x, threshold )= ( x / threshold) ^2.
    //threshold gibt an, an welcher stelle 1 zurück gegeben wird, wenn x = threshold ist. 
    //x Werte unter threshold wird dann nach 0 auf 0 gemapped. 
    //x Werte über threshold wird nach unendlich auf 1 gemapped. 
    static double QuadraticNormalization(double x, double threshold)
    {
        if (x >= threshold)
            return 1;
        return (x / threshold) * (x / threshold);
    }

    //f( x, threshold )= -x + 1
    //threshold gibt an, an welcher stelle 0 zurück gegeben wird, wenn x = threshold ist. 
    //x Werte unter threshold wird dann nach 0 auf 1 und darunter auf 1 bleiben gemapped.
    //x Werte über threshold wird nach unendlich auf 0 gemapped.
    static double CalculateLinearThresholdFunction(double x, double threshold)
    {
        if (x > threshold)
            return 0.0;
        else if (x < 0)
            return 1;
        else
            return 1 - x;
    }

    static double Clamp(double b, double max, double min)
    {
        if (b < min)
            return min;
        else if (b > max)
            return max;
        return b;
    }
};


