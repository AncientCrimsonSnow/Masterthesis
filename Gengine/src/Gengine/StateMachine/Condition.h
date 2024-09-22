#pragma once

#include "Gengine/Utils/Action.h"
#include "Usings.h"

namespace Gengine {
	class Condition {
	public:
		virtual ~Condition() { RemoveAllListeners(); }

		void AddListener(Listener<void> listener) { m_OnConditionMeetEvent.AddListener(listener); }
		void RemoveListener(Listener<void> listener) { m_OnConditionMeetEvent.RemoveListener(listener); }
		void RemoveAllListeners() { m_OnConditionMeetEvent.RemoveAllListeners(); }

		virtual bool operator==(const Condition& other) const = 0;
	protected:
		Action<void> m_OnConditionMeetEvent;
	};
}