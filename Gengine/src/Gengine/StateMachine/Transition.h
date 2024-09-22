#pragma once

#include <memory>
#include "State.h"
#include "Condition.h"
#include "StateMachineManager.h"

namespace Gengine {

    class StateMachineManager;
    class State;

    class Transition : public std::enable_shared_from_this<Transition> {
    public:
        Transition(
            std::shared_ptr<StateMachineManager> stateMachineManager,
            std::shared_ptr<State> from,
            std::shared_ptr<State> to,
            std::shared_ptr<Condition> condition);

        ~Transition();

        std::shared_ptr<State> GetFrom();
        std::shared_ptr<State> GetTo();

        void Execute();

        bool operator==(const Transition& other) const {
            return m_From == other.m_From
                && m_To == other.m_To
                && m_Condition == other.m_Condition;
        }

    private:
        std::shared_ptr<StateMachineManager> m_StateMachineManager;
        std::shared_ptr<State> m_From;
        std::shared_ptr<State> m_To;
        std::shared_ptr<Condition> m_Condition;
    };

}
