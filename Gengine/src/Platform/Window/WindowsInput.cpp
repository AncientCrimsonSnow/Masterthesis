#include "ggpch.h"
#include "WindowsInput.h"
#include "Gengine/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Gengine {

	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keyCode) {
		var window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		var state = glfwGetKey(window, keyCode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseBtnPressedImpl(int btn) {
		var window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		var state = glfwGetMouseButton(window, btn);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePosImpl() {
		var window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { (float)xPos, (float)yPos };
	}
}