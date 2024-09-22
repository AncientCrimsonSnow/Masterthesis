#pragma once

#include "Event.h"

namespace Gengine {

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) {}
		
		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMvd)
		EVENT_CLASS_CATEGORY(ECMouse | ECInput)

	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(ECMouse | ECInput)

	private:
		float m_XOffset, m_YOffset;
	};

	class MouseBtnEvent : public Event {
	public:
		inline int GetMouseBtn() const { return m_Btn; }

		EVENT_CLASS_CATEGORY(ECMouse | ECInput)

	protected:
		MouseBtnEvent(int btn)
			: m_Btn(btn) {}
		int m_Btn;
	};

	class MouseBtnPrsdEvent : public MouseBtnEvent {
	public:
		MouseBtnPrsdEvent(int btn)
			: MouseBtnEvent(btn) {}

		string ToString() const override {
			std::stringstream ss;
			ss << "MouseBtnPrsdEvent: " << m_Btn;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseBtnPrsd)
	};

	class MouseBtnRlsdEvent : public MouseBtnEvent {
	public:
		MouseBtnRlsdEvent(int btn)
			: MouseBtnEvent(btn) {}

		string ToString() const override {
			std::stringstream ss;
			ss << "MouseBtnRlsdEvent: " << m_Btn;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseBtnRlsd)
	};
}