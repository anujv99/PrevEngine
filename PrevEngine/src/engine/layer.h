#pragma once

#include "engine/events/event.h"

namespace prev {

	class Layer {
		friend class Application;
		friend class LayerStack;
	public:
		Layer(const std::string &name = "Layer");
		virtual ~Layer();
	private:
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event &event) {}

		inline const std::string &GetName() const {	return m_DebugName;	}
	private:
		std::string m_DebugName;
	};

}