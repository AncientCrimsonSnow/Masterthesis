#pragma once

#include "Gengine/Renderer/GrapihcsContext.h"
#include <GLFW/glfw3.h>

class GLFWwindow;

namespace Gengine {
	struct OpenGlContext : public GraphicsContext {

	public:
		OpenGlContext(GLFWwindow* windowhandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}