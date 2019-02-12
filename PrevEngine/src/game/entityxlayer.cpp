#include "pch.h"
#include "entityxlayer.h"

namespace prev {

	EntityXLayer::EntityXLayer() {
		this->systems.add<entityx::deps::Dependency<components::Renderable, components::Position, components::Scale>>();
		this->systems.add<entityx::deps::Dependency<components::Collision, components::Position, components::Scale>>();
		systems.configure();
	}

	void EntityXLayer::Update() {
		this->systems.update_all(Timer::GetDeltaTime());
	}

}