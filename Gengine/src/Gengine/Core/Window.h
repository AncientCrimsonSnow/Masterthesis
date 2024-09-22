#pragma once

#include "ggpch.h"

#include "Gengine/Core/Core.h"
#include "Gengine/Events/Event.h"

namespace Gengine {
	struct WindowProps {
		string Title;
		uint Width;
		uint Height;

		WindowProps(
			const string& title = "Gengine", 
			uint width = 1400, 
			uint height = 700) 
			: Title(title), Width(width), Height(height) {}
	};

	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void Update() = 0;

		virtual uint GetWidth() const = 0;
		virtual uint GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVsync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
