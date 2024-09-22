#pragma once

#include "Gengine/Core/Core.h"

namespace Gengine {
	class Input {
	public:
		inline static bool IsKeyPressed(int keyCode) {
			return s_Instance->IsKeyPressedImpl(keyCode);
		}

		inline static bool IsMouseBtnPressed(int btn) {
			return s_Instance->IsMouseBtnPressedImpl(btn);
		}

		inline static std::pair<float, float> GetMousePos() {
			return s_Instance->GetMousePosImpl();
		}

	protected:
		virtual bool IsKeyPressedImpl(int keyCode) = 0;
		virtual bool IsMouseBtnPressedImpl(int btn) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
	private:
		static Input* s_Instance;
	};
}