#pragma once

#include <string>
#include <functional>

template <typename T = void>
using Listener = std::function<void(T)>;

using string = std::string;
using uint = unsigned int;
