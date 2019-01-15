#pragma once

#ifdef PV_RENDERING_API_OPENGL

#include "../api.h"

namespace prev { namespace graphicsapi { namespace opengl {

	class OpenGL : public API {
	public:
		OpenGL(unsigned int windowWidth, unsigned int windowHeight);
		~OpenGL();

		void OnUpdate() override;
		void OnEvent(EventDispatcher &dispatcher) override;
	private:
		void SetViewport() override;
	};

} } }

#endif