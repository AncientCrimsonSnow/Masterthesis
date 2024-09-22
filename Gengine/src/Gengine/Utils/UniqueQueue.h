#pragma once

#include "ggpch.h"

namespace Gengine {
    template <typename T>
    class UniqueQueue {
    public:
        bool Enqueue(const T& element) {
            if (!Contains(element)) {
                m_Queue.push(element);
                return true;
            }
            return false;
        }

        bool Dequeue() {
            if (!m_Queue.empty()) {
                m_Queue.pop();
                return true;
            }
            return false;
        }

        T Front() const {
            if (!m_Queue.empty()) {
                return m_Queue.front();
            }
            throw std::runtime_error("Queue is empty");
        }

        bool Empty() const {
            return m_Queue.empty();
        }

        size_t Size() const {
            return m_Queue.size();
        }

        void Clear() {
            while (!m_Queue.empty()) {
                m_Queue.pop();
            }
        }
    private:
        bool Contains(const T& element) const {
            std::queue<T> tempQueue = m_Queue;
            while (!tempQueue.empty()) {
                if (tempQueue.front() == element) {
                    return true;
                }
                tempQueue.pop();
            }
            return false;
        }

        std::queue<T> m_Queue;
    };
}
