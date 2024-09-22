#include "ggpch.h"
#include "StateMachineManager.h"
#include "State.h"

namespace Gengine {

    void StateMachineManager::AddStates(std::initializer_list<std::shared_ptr<State>> states) {
        for (const std::shared_ptr<State>& state : states)
            AddState(state);
    }
    void StateMachineManager::AddState(std::shared_ptr<State> state) {
        if (m_AllStates.Contains(state))
            return;

        state->SetManager(shared_from_this());
        m_InitialisationQueue.Enqueue(state);

        if (state->GetParent()) {
            state->GetParent()->AddChild(state);
            m_LeafStates.TryRemove(state->GetParent());
        }

        m_LeafStates.push_back(state);
        m_AllStates.push_back(state);

        std::stack<std::shared_ptr<State>> stack;
        if (!m_CurrentState) {
            std::shared_ptr<State> crrState = state;
            while (crrState) {
                stack.push(crrState);
                crrState = crrState->GetParent();
            }
            while (!stack.empty()) {
                m_EnterQueue.Enqueue(stack.top());
                stack.pop();
            }
            m_CurrentState = state;
        }
        else if (IsParentOf(state, m_CurrentState)) {
            std::shared_ptr<State> crrState = state;
            while (crrState != m_CurrentState) {
                stack.push(crrState);
                crrState = crrState->GetParent();
            }
            while (!stack.empty()) {
                m_EnterQueue.Enqueue(stack.top());
                stack.pop();
            }
            m_CurrentState = state;
        }
    }

    void StateMachineManager::Update(Timestep deltaTime) {
        ProcessQueues();
        std::shared_ptr<State> state = m_CurrentState;
        while (state != nullptr) {
            state->Update(deltaTime);
            state = state->GetParent();
        }
    }

    void StateMachineManager::ImGuiRender() {
        std::shared_ptr<State> state = m_CurrentState;
        while (state != nullptr) {
            state->ImGuiRender();
            state = state->GetParent();
        }
    }

    void StateMachineManager::SwitchState(std::shared_ptr<State> state) {
        if (state == m_CurrentState) {
            GG_CORE_WARN("State {0} is already active", static_cast<void*>(state.get()));
            return;
        }

        while (!m_LeafStates.Contains(state)) {
            if (!state->LastChild) {
                state->LastChild = state->Children.front();
            }
            state = state->LastChild;
        }

        var [pathState1ToLCA, pathLCAtoState2] = FindPathsToLCA(m_CurrentState, state);

        for (var& pathState : pathState1ToLCA)
            m_ExitQueue.Enqueue(pathState);

        for (var& pathState : pathLCAtoState2)
            m_EnterQueue.Enqueue(pathState);

        while (state->GetParent()) {
            state->GetParent()->LastChild = state;
            state = state->GetParent();
        }
    }

    bool StateMachineManager::IsParentOf(std::shared_ptr<State> child, std::shared_ptr<State> parent) {
        std::shared_ptr<State> current = child;
        while (current->GetParent()) {
            if (current->GetParent() == parent)
                return true;
            current = current->GetParent();
        }
        return false;
    }

    int StateMachineManager::GetDepth(std::shared_ptr<State> state) {
        int depth = 0;
        while (state) {
            state = state->GetParent();
            depth++;
        }
        return depth;
    }

    std::pair<Vector<std::shared_ptr<State>>, Vector<std::shared_ptr<State>>> StateMachineManager::FindPathsToLCA(std::shared_ptr<State> state1, std::shared_ptr<State> state2) {
        Vector<std::shared_ptr<State>> path1;
        Vector<std::shared_ptr<State>> path2;

        int depth1 = GetDepth(state1);
        int depth2 = GetDepth(state2);

        while (depth1 > depth2) {
            path1.push_back(state1);
            state1 = state1->GetParent();
            depth1--;
        }

        while (depth2 > depth1) {
            path2.insert(path2.begin(), state2);
            state2 = state2->GetParent();
            depth2--;
        }

        while (state1 && state2 && state1 != state2) {
            path1.push_back(state1);
            path2.insert(path2.begin(), state2);
            state1 = state1->GetParent();
            state2 = state2->GetParent();
        }

        return std::make_pair(path1, path2);
    }

    void StateMachineManager::ProcessInitQueue() {
        while (!m_InitialisationQueue.Empty()) {
            m_InitialisationQueue.Front()->Init();
            m_InitialisationQueue.Dequeue();
        }
    }

    void StateMachineManager::ProcessTransitionAddQueue() {
        UniqueQueue<std::shared_ptr<TransitionData>> transitionAddQueueCopy = m_TransitionAddQueue;
        m_TransitionAddQueue.Clear();

        while (!transitionAddQueueCopy.Empty()) {
            std::shared_ptr<TransitionData> transitionData = transitionAddQueueCopy.Front();
            transitionAddQueueCopy.Dequeue();

            transitionData->from->CreateTransition(transitionData->to, transitionData->condition);
        }
    }

    void StateMachineManager::ProcessTransitionExecuteQueue() {
        UniqueQueue<std::shared_ptr<Transition>> transitionExecuteQueueCopy = m_TransitionExecuteQueue;
        m_TransitionExecuteQueue.Clear();

        while (!transitionExecuteQueueCopy.Empty()) {
            std::shared_ptr<Transition> transition = transitionExecuteQueueCopy.Front();
            transitionExecuteQueueCopy.Dequeue();

            std::shared_ptr<State> fromState = transition->GetFrom();
            if (!(m_CurrentState == fromState)) {
                if (m_CurrentState->IsChildOf(fromState))
                    SwitchState(transition->GetTo());
                else
                    continue;
            }
            SwitchState(transition->GetTo());
        }
    }

    void StateMachineManager::ProcessExitQueue() {
        UniqueQueue<std::shared_ptr<State>> exitQueueCopy = m_ExitQueue;
        m_ExitQueue.Clear();
        while (!exitQueueCopy.Empty()) {
            std::shared_ptr<State> state = exitQueueCopy.Front();
            exitQueueCopy.Dequeue();
            state->Exit();
        }
    }

    void StateMachineManager::ProcessEnterQueue() {
        UniqueQueue<std::shared_ptr<State>> enterQueueCopy = m_EnterQueue;
        m_EnterQueue.Clear();

        std::shared_ptr<State> newCurrentState = nullptr;
        while (!enterQueueCopy.Empty()) {
            newCurrentState = enterQueueCopy.Front();
            enterQueueCopy.Dequeue();
            newCurrentState->Enter();
        }

        if (newCurrentState)
            m_CurrentState = newCurrentState;
    }
}
