#pragma once

#include "ggpch.h"
#include "Vector.h"

namespace Gengine {
    template <typename T = void>
    class Action {
    public:
        ~Action() { RemoveAllListeners(); }

        void AddListener(const Listener<T>& listener) { listeners.push_back(listener); }

        void Invoke(T value) {
            for (var& listener : listeners)
                listener(value);
        }

        void RemoveListener(const Listener<T>& listener) {
            listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
                [&](const Listener<T>& h) {
                    return h.target<void(T)>() == listener.target<void(T)>();
                }), listeners.end());
        }

        void RemoveAllListeners() { listeners.clear(); }

        bool operator==(const Action<T>& other) const {
            if (listeners.size() != other.listeners.size()) {
                return false;
            }

            for (size_t i = 0; i < listeners.size(); ++i) {
                if (!(listeners[i] == other.listeners[i])) {
                    return false;
                }
            }
            return true;
        }

    private:
        Vector<Listener<T>> listeners;
    };

    template <>
    class Action<void> {
    public:
        ~Action() { RemoveAllListeners(); }

        void AddListener(const Listener<void>& listener) { 
            listeners.push_back(listener); 
        }

        void Invoke() {
            for (var& listener : listeners)
                listener();
        }

        void RemoveListener(const Listener<void>& listener) {
            listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
                [&](const Listener<void>& h) {
                    return h.target<void()>() == listener.target<void()>();
                }), listeners.end());
        }

        void RemoveAllListeners() { listeners.clear(); }

        bool operator==(const Action<void>& other) const {
            if (listeners.size() != other.listeners.size()) {
                return false;
            }

            for (size_t i = 0; i < listeners.size(); ++i) {
                if (!(listeners[i].target_type() == other.listeners[i].target_type())) {
                    return false;
                }
            }
            return true;
        }

    private:
        Gengine::Vector<Listener<void>> listeners;
    };
}