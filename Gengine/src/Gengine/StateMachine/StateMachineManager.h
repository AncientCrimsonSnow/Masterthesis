#pragma once

#include "State.h"
#include "Gengine/Utils/UniqueQueue.h"

namespace Gengine {

    class State;
    class Transition;

    class StateMachineManager : public std::enable_shared_from_this<StateMachineManager> {
    public:
        struct TransitionData {
        public:
            std::shared_ptr<State> from;
            std::shared_ptr<State> to;
            std::shared_ptr<Condition> condition;

            TransitionData(
                std::shared_ptr<State> from, 
                std::shared_ptr<State> to, 
                std::shared_ptr<Condition> condition)
                : 
                from(from), 
                to(to), 
                condition(condition) {
            }

            bool operator==(const TransitionData& other) const {
                return (from == other.from) && (to == other.to) && (condition == other.condition);
            }
        };
        StateMachineManager() = default;
        ~StateMachineManager() = default;

        void AddState(std::shared_ptr<State> state);
        void AddStates(std::initializer_list<std::shared_ptr<State>> states);
        void Update(Timestep deltaTime);
        void ImGuiRender();

        template<typename T>
        bool TryGetState(std::shared_ptr<T>& outState) const {
            GG_ASSERT(std::is_base_of<State, T>::value, "T must inherit from State" );
            for (const var& state : m_AllStates) {
                if (std::shared_ptr<T> castedState = std::dynamic_pointer_cast<T>(state)) {
                    outState = castedState;
                    return true;
                }
            }
            outState.reset();
            return false;
        }

        std::shared_ptr<State> GetCurrentState() { return m_CurrentState; }
        void QueTransition(std::shared_ptr<Transition> transition) { m_TransitionExecuteQueue.Enqueue(transition); }

        void AddTransition(TransitionData data) {
            m_TransitionAddQueue.Enqueue(std::make_shared<TransitionData>(data));
        }

        void AddTransitions(std::initializer_list<TransitionData> TransitionDatas) {
            for (const TransitionData& transitionData : TransitionDatas)
                AddTransition(transitionData);
        }

    private:

        std::shared_ptr<State> m_CurrentState;
        UniqueQueue<std::shared_ptr<Transition>> m_TransitionExecuteQueue;

        Vector<std::shared_ptr<State>> m_AllStates;
        Vector<std::shared_ptr<State>> m_LeafStates;

        UniqueQueue<std::shared_ptr<State>> m_InitialisationQueue;
        UniqueQueue<std::shared_ptr<State>> m_EnterQueue;
        UniqueQueue<std::shared_ptr<State>> m_ExitQueue;

        UniqueQueue<std::shared_ptr<TransitionData>> m_TransitionAddQueue;

        void SwitchState(std::shared_ptr<State> state);
        void ProcessInitQueue();
        void ProcessTransitionAddQueue();
        void ProcessTransitionExecuteQueue();
        void ProcessExitQueue();
        void ProcessEnterQueue();

        bool IsParentOf(std::shared_ptr<State> child, std::shared_ptr<State> parent);
        int GetDepth(std::shared_ptr<State> state);
        std::pair<Vector<std::shared_ptr<State>>, Vector<std::shared_ptr<State>>> FindPathsToLCA(std::shared_ptr<State> state1, std::shared_ptr<State> state2);

        void ProcessQueues() {
            ProcessInitQueue();
            ProcessTransitionAddQueue();
            ProcessTransitionExecuteQueue();
            ProcessExitQueue();
            ProcessEnterQueue();
        }
    };
}