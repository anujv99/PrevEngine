#pragma once

#include "engine/layer.h"
#include "game/predefinedcomponents.h"
#include "game/predefinedsystems.h"

namespace prev {

	class EntityXLayer final : public entityx::EntityX, public Layer {
	public:
		EntityXLayer(std::string layerName = "EntityXLayer");

		template <typename T>
		void AddSystem() {
			this->systems.add<T>();
			this->systems.configure();
		}
	private:
		//Inherited Methods
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event & e) override;
	};

}