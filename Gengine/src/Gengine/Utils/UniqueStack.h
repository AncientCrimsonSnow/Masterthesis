#pragma once
#include "ggpch.h"

namespace Gengine {
    template <typename T>
    class UniqueStack {
    public:
        bool Push(const T& element) {
            if (!Contains(element)) {
                m_Stack.push(element);
                return true;
            }
            return false;
        }

        bool Pop() {
            if (!m_Stack.empty()) {
                m_Stack.pop();
                return true;
            }
            return false;
        }

        T Top() const {
            if (!m_Stack.empty()) {
                return m_Stack.top();
            }
            throw std::runtime_error("Stack is empty");
        }

        bool Empty() const {
            return m_Stack.empty();
        }

        size_t Size() const {
            return m_Stack.size();
        }

        void Clear() {
            while (!m_Stack.empty()) {
                m_Stack.pop();
            }
        }
    private:
        bool Contains(const T& element) const {
            std::stack<T> tempStack = m_Stack;
            while (!tempStack.empty()) {
                if (tempStack.top() == element) {
                    return true;
                }
                tempStack.pop();
            }
            return false;
        }

        std::stack<T> m_Stack;
    };
}
