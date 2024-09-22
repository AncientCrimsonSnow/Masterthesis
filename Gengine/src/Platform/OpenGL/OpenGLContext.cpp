#include "ggpch.h"
#include "OpenGlContext.h"

#include "GLFW/glfw3.h"
#include <glad/glad.h>

namespace Gengine {
	OpenGlContext::OpenGlContext(GLFWwindow* windowhandle)
		: m_WindowHandle(windowhandle)	 {
		GG_CORE_ASSERT(windowhandle, "Window handle is null!")
	}
	void OpenGlContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GG_CORE_ASSERT(status, "Failed to initialize Glad!")

		GG_CORE_INFO("OpenGL Info:");
		GG_CORE_INFO("    Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		GG_CORE_INFO("    Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		GG_CORE_INFO("    Version: {0}", (const char*)glGetString(GL_VERSION));

	}
	void OpenGlContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}
}