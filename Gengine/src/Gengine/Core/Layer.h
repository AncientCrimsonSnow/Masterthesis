#pragma once

#include "Gengine/Core/Core.h"
#include "Gengine/Events/Event.h"
#include "Gengine/Core/Timestep.h"


namespace Gengine {
	class Layer {
	public:
		Layer(const string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void Update(Timestep deltaTime) {}
		virtual void ImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const string& GetName() const { return m_DebugName; }
	protected:
		string m_DebugName;
	};
}


