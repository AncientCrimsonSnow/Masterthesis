#pragma once

#include "ggpch.h"
#include "Gengine/Core/Core.h"

namespace Gengine {

	// Btn = Button
	// Prsd = Pressed
	// Rlsd = Released
	// Mvd = Moved
	// App = Application
	// EC = EventCategory

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMvd,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseBtnPrsd, MouseBtnRlsd, MouseMvd, MouseScrolled
	};

	//Note: BIT to get a Bitfield which allowed us to add multiple EC's
	enum EventCategory {
		None = 0,
		ECApp		= BIT(0),
		ECInput		= BIT(1),
		ECKeyBoard	= BIT(2),
		ECMouse		= BIT(3),
		ECMouseBtn	= BIT(4)
	};

	//Note: Makros um Methoden in Klassen hinzuzufügen, die auch statisch sein können. Vor dem Compiler. 
	//Note: "#" converts something into a string
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


	class Event {
		//Note "friend": EventDispatcher has access to private and protected members
		friend class EventDispatcher;
	public:
		//Note "const" after functiondeclaration: Implementation can't change state of Instance
		virtual EventType GetEventType() const = 0;
		//Note "const" before returnType: Returnvalue on which the pointer is poiting to cant be changed
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		//Note "virtual" without returntype => abstract, with returntype => virtual
		virtual string ToString() const { return GetName(); }
		//Note "inlane" compiler puts this into the calledspace and not a ref
		inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }
		bool Handled = false;
	};

	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
			EventDispatcher(Event& event)
				: m_Event(event){}

			template<typename T>
			bool Dispatch(EventFn<T> func) {
				if (m_Event.GetEventType() == T::GetStaticType()) {
					m_Event.Handled = func(*(T*)&m_Event);
					return true;
				}
				return false;					
			}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
}