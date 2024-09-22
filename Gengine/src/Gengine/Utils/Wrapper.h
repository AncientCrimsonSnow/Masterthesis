#pragma once+

#include "Gengine/Utils/Action.h"

namespace Gengine {
    template <typename T>
    class Wrapper 
    {
    public:

        Wrapper(T value) : m_Value(value) 
        {}

        T GetValue() const 
        { 
            return m_Value; 
        }

        void SetValue(T value) 
        {
            if (m_Value != value) 
            {
                m_Value = value;
                NotifyValueChanged();
            }
        }

        void RegisterOnValueChanged(Listener<T> listener) 
        { 
            m_ValueChangedEvent.AddListener(listener); 
        }
        void UnregisterOnValueChanged(Listener<T> listener)
        { 
            m_ValueChangedEvent.RemoveListener(listener); 
        }

        bool operator==(const Wrapper<T>& other) const 
        { 
            return Compare(m_Value, other.m_Value);
        }
    private:
        T m_Value;
        Action<T> m_ValueChangedEvent;

        void NotifyValueChanged() 
        { 
            m_ValueChangedEvent.Invoke(m_Value); 
        }

        static bool Compare(const T& a, const T& b) 
        {
            return CompareImpl(a, b, std::is_pointer<T>());
        }
        static bool CompareImpl(const T& a, const T& b, std::true_type) 
        {
            return a == b;
        }

        static bool CompareImpl(const T& a, const T& b, std::false_type) 
        {
            return &a == &b;
        }
    };
}

