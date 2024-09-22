#include "ggpch.h"
#include "Time.h"
#include <GLFW/glfw3.h>

float Gengine::Time::GetRealtimeSinceStart() { return (float)glfwGetTime(); }
