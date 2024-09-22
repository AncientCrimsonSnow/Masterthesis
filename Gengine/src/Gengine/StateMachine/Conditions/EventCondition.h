#pragma once

#include "Gengine/Utils/Action.h"
#include "Gengine/StateMachine/Condition.h"

namespace Gengine {
    class EventCondition : public Condition {
    public:
        EventCondition(Action<>& event) : m_Event(event) {
            event.AddListener(GG_BIND_EVENT_FN(EventCondition::TriggerConditionMeetWrapper));
        }

        bool operator==(const Condition& other) const override {
            const EventCondition* otherCondition = dynamic_cast<const EventCondition*>(&other);
            if (!otherCondition) {
                return false;
            }
            return &m_Event == &otherCondition->m_Event;
        }

    private:
        void TriggerConditionMeetWrapper() {
            m_OnConditionMeetEvent.Invoke();
        }

        Action<>& m_Event;
    };
}