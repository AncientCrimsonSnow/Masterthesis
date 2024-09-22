#include "ggpch.h"
#include "LayerStack.h"

Gengine::LayerStack::LayerStack() {
}

Gengine::LayerStack::~LayerStack() {
	for (Layer* layer : m_Layers) {
		layer->OnDetach();
		delete layer;
	}
}

void Gengine::LayerStack::PushLayer(Layer* layer) {
	m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
	m_LayerInsertIndex++;
	layer->OnAttach();
}

void Gengine::LayerStack::PushOverlay(Layer* overlay) {
	m_Layers.emplace_back(overlay);
	overlay->OnAttach();
}

void Gengine::LayerStack::PopLayer(Layer* layer) {
	var it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
	if (it != m_Layers.end()) {
		layer->OnDetach();
		m_Layers.erase(it);
		m_LayerInsertIndex--;
	}
}

void Gengine::LayerStack::PopOverlay(Layer* overlay) {
	var it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
	if (it != m_Layers.end()) {
		overlay->OnDetach();
		m_Layers.erase(it);
	}
}
