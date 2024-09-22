#pragma once

#include "Gengine/Core/Core.h"
#include "Layer.h"
#include "Gengine/Utils/Vector.h"

namespace Gengine {

	class LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		Vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		Vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		Vector<Layer*> m_Layers;
		uint m_LayerInsertIndex = 0;
	};
}


