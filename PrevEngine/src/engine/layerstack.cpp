#include "pch.h"
#include "layerstack.h"

namespace prev {

	LayerStack::LayerStack() {}

	LayerStack::~LayerStack() {
		for (Layer * layer : m_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer * layer) {
		m_Layers.push_back(layer);
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer * overlay) {
		m_Overlays.push_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer * layer) {
		for (int i = 0; i < m_Layers.size(); i++) {
			if (m_Layers[i] == layer) {
				m_Layers.erase(m_Layers.begin() + i);
				layer->OnDetach();
			}
		}
	}

	void LayerStack::PopOverlay(Layer * layer) {
		for (int i = 0; i < m_Overlays.size(); i++) {
			if (m_Overlays[i] == layer) {
				m_Overlays.erase(m_Overlays.begin() + i);
				layer->OnDetach();
			}
		}
	}

	void LayerStack::OnUpdate() {
		for (auto layer : m_Layers) {
			layer->OnUpdate();
		}

		for (auto layer : m_Overlays) {
			layer->OnUpdate();
		}
	}

	void LayerStack::OnEvent(Event & e) {
		for (auto layer : m_Layers) {
			layer->OnEvent(e);
			if (e.Handled())
				break;
		}

		for (auto layer : m_Overlays) {
			layer->OnEvent(e);
			if (e.Handled())
				break;
		}
	}

}