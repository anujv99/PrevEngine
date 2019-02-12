#pragma once

#include "game/predefinedcomponents.h"
#include "game/predefinedsystems.h"

namespace prev {

	class EntityXLayer : public entityx::EntityX {
		friend class Level;
	private:
		EntityXLayer();
		void Update();
	public:
		template <typename T>
		void AddSystem() {
			this->systems.add<T>();
			this->systems.configure();
		}

	};

}