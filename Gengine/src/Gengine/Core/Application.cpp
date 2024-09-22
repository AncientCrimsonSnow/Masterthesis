#include "ggpch.h"
#include "Application.h"

#include "Input.h"
#include "Gengine/Renderer/Renderer.h"

#include "Gengine/Core/Timestep.h"

#include <GLFW/glfw3.h>

namespace Gengine {

	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{
		GG_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(GG_BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(false);

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(GG_BIND_EVENT_FN(Application::OnWindowClose));

		for (var it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
	}

	void Application::Run() {
		while (m_Running) {		

			float time = (float)glfwGetTime(); //TODO: Platform::GetTime
			Timestep deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->Update(deltaTime);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->ImGuiRender();
			m_ImGuiLayer->End();

			m_Window->Update();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
