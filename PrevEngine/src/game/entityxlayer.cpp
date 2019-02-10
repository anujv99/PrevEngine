#include "pch.h"
#include "entityxlayer.h"

namespace prev {

	EntityXLayer::EntityXLayer(std::string layerName) :
		Layer(layerName) {
		this->systems.add<entityx::deps::Dependency<components::Renderable, components::Position, components::Scale>>();
		systems.configure();
	}

	void EntityXLayer::OnAttach() {
	}

	void EntityXLayer::OnDetach() {
	}

	void EntityXLayer::OnUpdate() {
		this->systems.update_all(Timer::GetDeltaTime());
	}

	void EntityXLayer::OnEvent(Event & e) {
	}

}