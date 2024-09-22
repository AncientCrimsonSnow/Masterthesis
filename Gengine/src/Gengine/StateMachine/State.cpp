#include "ggpch.h"
#include "State.h"
#include "StateMachineManager.h"

namespace Gengine {
	void State::ClearTransition() {
		for (std::shared_ptr<Transition> transition : m_Transitions) {
			transition->~Transition();
		}
	}
	void State::CreateTransition(std::shared_ptr<State> to, std::shared_ptr<Condition> condition) {
        
        var transition = std::make_shared<Transition>(
            m_StateMachineManager,
            shared_from_this(),
            to,
            condition);

        if (!m_Transitions.Contains(transition))
            m_Transitions.push_back(transition);
    }
}