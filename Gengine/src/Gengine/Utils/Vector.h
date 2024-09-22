#pragma once

#include <vector>
#include <algorithm>
#include <sstream>
#include <type_traits>

namespace Gengine {
    template <typename T>
    class Vector : public std::vector<T> {
    public:

        using std::vector<T>::vector;

        template <typename InputIt>
        Vector(InputIt first, InputIt last) : std::vector<T>(first, last) {}

        bool Contains(const T& element) {
            size_t size = this->size();
            if (size == 0)
                return false;
            auto it = std::find(this->begin(), this->end(), element);
            return it != this->end();
        }

        bool TryRemove(T value) {
            size_t originalSize = this->size();
            if (originalSize == 0)
                return false;
            this->erase(std::remove(this->begin(), this->end(), value), this->end());
            return this->size() < originalSize;
        }

        bool TryGetIndexOf(const T& element, size_t& index) const {
            auto it = std::find(this->begin(), this->end(), element);
            if (it != this->end()) {
                index = std::distance(this->begin(), it);
                return true;
            }
            else {
                return false;
            }
        }

        void Append(const Vector<T>& other)
        {
            if (!other.empty())
            {
                this->reserve(this->size() + other.size());
                this->insert(this->end(), other.begin(), other.end());
            }
        }

        T PopBack()
        {
            T lastElement = this->back();
            this->pop_back();
            return lastElement;
        }
    };
}
