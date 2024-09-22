#pragma once

#include "Core.h"
#include "Window.h"
#include "Gengine/Core/LayerStack.h"
#include "Gengine/Events/Event.h"
#include "Gengine/Events/ApplicationEvent.h"

#include "Gengine/ImGui/ImGuiLayer.h"

#include "Gengine/Renderer/Shader.h"
#include "Gengine/Renderer/Buffer.h"
#include "Gengine/Renderer/VertexArray.h"
#include "Gengine/Renderer/Camera/OrthographicCamera.h"

namespace Gengine {
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0;

		static Application* s_Instance;
	};


	Application* CreateApplication();
}
