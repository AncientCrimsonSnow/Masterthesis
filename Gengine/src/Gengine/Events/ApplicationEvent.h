#pragma once

#include "Event.h"

namespace Gengine {
	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(uint width, uint height)
			: m_Width(width), m_Height(height) {}

		inline uint GetWidth() const { return m_Width; }
		inline uint GetHeight() const { return m_Height; }

		string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(ECApp)

	private:
		uint m_Width, m_Height;
	};

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(ECApp)
	};

	class AppTickEvent : public Event {
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(ECApp)
	};

	class AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(ECApp)
	};

	class AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
	    EVENT_CLASS_CATEGORY(ECApp)
	};
}