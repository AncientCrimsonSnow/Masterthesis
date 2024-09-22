#pragma once

#include "Gengine/StateMachine/Condition.h"
#include "Gengine/Utils/Wrapper.h"

namespace Gengine {
    class BooleanCondition : public Condition {

    public:
        BooleanCondition(std::shared_ptr<Wrapper<bool>> booleanWrapper) : m_BooleanWrapper(booleanWrapper) {
            m_BooleanWrapper->RegisterOnValueChanged(GG_BIND_EVENT_FN(BooleanCondition::TryTriggerConditionMeet));
        }

        ~BooleanCondition() {
            m_BooleanWrapper->UnregisterOnValueChanged(GG_BIND_EVENT_FN(BooleanCondition::TryTriggerConditionMeet));
        }

        bool operator==(const Condition& other) const override {
            const BooleanCondition* otherCondition = dynamic_cast<const BooleanCondition*>(&other);
            if (!otherCondition) {
                return false;
            }
            return m_BooleanWrapper == otherCondition->m_BooleanWrapper;
        }

    private:
        std::shared_ptr<Wrapper<bool>> m_BooleanWrapper;

        void TryTriggerConditionMeet(bool value) {
            if (value)
                m_OnConditionMeetEvent.Invoke();
        }
    };
}
