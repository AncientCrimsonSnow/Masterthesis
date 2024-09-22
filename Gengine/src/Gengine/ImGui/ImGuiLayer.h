#pragma once

#include "Gengine/Core/Layer.h"

#include "Gengine/Events/MouseEvent.h"
#include "Gengine/Events/ApplicationEvent.h"
#include "Gengine/Events/KeyEvent.h"

namespace Gengine {
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override; 
		virtual void OnDetach() override; 
		virtual void ImGuiRender() override; 

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}