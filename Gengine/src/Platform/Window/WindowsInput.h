#pragma once
#include "Gengine/Core/Input.h"

namespace Gengine {
	class WindowsInput : public Input {
	protected:
		bool IsKeyPressedImpl(int keyCode) override;
		bool IsMouseBtnPressedImpl(int btn) override;
		std::pair<float, float> GetMousePosImpl() override;
	};
}

