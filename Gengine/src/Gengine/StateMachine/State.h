#pragma once

#include "Gengine/Utils/Vector.h"
#include "Gengine/Utils/Wrapper.h"
#include "Gengine/Core/Timestep.h"
#include "Transition.h"
#include "Condition.h"

#include <memory>

namespace Gengine {

    class StateMachineManager;
    class Transition;

    class State : public std::enable_shared_from_this<State> {
    public:
        State(std::shared_ptr<State> parent = nullptr)
            : m_Parent(parent) {
        }

        std::shared_ptr<State> LastChild;
        Vector<std::shared_ptr<State>> Children;

        virtual void Init() { GG_INFO("Init {0}-State", ToString()); }
        virtual void Enter() { GG_INFO("Enter {0}-State", ToString()); }
        virtual void Update(Timestep deltaTime) { GG_INFO("Update {0}-State", ToString()); }
        virtual void ImGuiRender() { GG_INFO("ImGuiRender {0}-State", ToString()); }
        virtual void Exit() { GG_INFO("Exit {0}-State", ToString()); }

        std::shared_ptr<State> GetParent() const { return m_Parent; }
        void ClearTransition();

        void SetManager(std::shared_ptr<StateMachineManager> stateMachineManager) {
            m_StateMachineManager = stateMachineManager;
        };

        void AddChild(std::shared_ptr<State> state) {
            if (!Children.Contains(state))
                Children.push_back(state);
        }

        void CreateTransition(std::shared_ptr<State> to, std::shared_ptr<Condition> condition);

        bool IsChildOf(std::shared_ptr<State> parent) const {
            if (!m_Parent)
                return false;

            std::shared_ptr<State> crrParent = m_Parent;
            while (parent != crrParent) {
                if (!crrParent->m_Parent)
                    return false;
                crrParent = crrParent->m_Parent;
            }
            return true;
        }

        string ToString() {
            const char* typeName = typeid(*this).name();
            const char* prefix = "class ";
            if (strncmp(typeName, prefix, strlen(prefix)) == 0)
                typeName += strlen(prefix);
            return string(typeName);
        }

    private:
        std::shared_ptr<State> m_Parent;
        Vector<std::shared_ptr<Transition>> m_Transitions;
    protected:
        std::shared_ptr<StateMachineManager> m_StateMachineManager;
    };
}
