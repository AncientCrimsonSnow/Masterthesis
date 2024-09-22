#include "ggpch.h"
#include "Transition.h"
#include "StateMachineManager.h"

namespace Gengine {
    Transition::Transition(
        std::shared_ptr<StateMachineManager> stateMachineManager,
        std::shared_ptr<State> from,
        std::shared_ptr<State> to,
        std::shared_ptr<Condition> condition)
        : m_StateMachineManager(stateMachineManager),
        m_From(from),
        m_To(to),
        m_Condition(condition) {
        m_Condition->AddListener(GG_BIND_EVENT_FN(Transition::Execute));
    }

    Transition::~Transition() {
        m_Condition->RemoveListener(GG_BIND_EVENT_FN(Transition::Execute));
    }

    std::shared_ptr<State> Transition::GetFrom() {
        return m_From;
    }

    std::shared_ptr<State> Transition::GetTo() {
        return m_To;
    }

    void Transition::Execute() {
        m_StateMachineManager->QueTransition(shared_from_this());
    }
}
