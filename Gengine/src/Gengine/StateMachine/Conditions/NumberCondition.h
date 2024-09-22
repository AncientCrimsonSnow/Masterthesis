#pragma once

#include "ggpch.h"
#include "Gengine/Utils/Wrapper.h"
#include "Gengine/StateMachine/Condition.h"
#include "ComparisonType.h"

namespace Gengine {
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    class NumberCondition : public Condition {
    public:
        NumberCondition(
            std::shared_ptr<Wrapper<T>> numberWrapper,
            T targetValue, 
            ComparisonType comparisonType)
            : 
            m_NumberWrapper(numberWrapper),
            m_TargetValue(targetValue), 
            m_ComparisonType(comparisonType) {
            m_NumberWrapper->RegisterOnValueChanged([this](T newValue) {
                CheckCondition(newValue);
                });
        }

        bool operator==(const Condition& other) const override {
            const NumberCondition<T>* otherCondition = dynamic_cast<const NumberCondition<T>*>(&other);
            if (!otherCondition) {
                return false;
            }
            return m_NumberWrapper == otherCondition->m_NumberWrapper;
        }
    private:
        T m_TargetValue;
        ComparisonType m_ComparisonType;
        std::shared_ptr<Wrapper<T>> m_NumberWrapper;

        void CheckCondition(T newValue) {
            bool conditionMet = false;

            switch (m_ComparisonType) {
            case ComparisonType::Equal:
                conditionMet = newValue == m_TargetValue;
                break;
            case ComparisonType::LessThan:
                conditionMet = newValue < m_TargetValue;
                break;
            case ComparisonType::GreaterThan:
                conditionMet = newValue > m_TargetValue;
                break;
            case ComparisonType::LessThanOrEqual:
                conditionMet = newValue <= m_TargetValue;
                break;
            case ComparisonType::GreaterThanOrEqual:
                conditionMet = newValue >= m_TargetValue;
                break;
            }

            if (conditionMet) {
                m_OnConditionMeetEvent.Invoke();
            }
        }
    };
}