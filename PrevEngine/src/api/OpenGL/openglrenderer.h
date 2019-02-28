#pragma once

#include "openglobjectsmanager.h"

namespace prev { namespace opengl {

	struct RawModel {
		const Vao * m_Vao;
		unsigned int m_VertexCount;
	};

	class OpenGLRenderer {
		friend class OpenGL;
	public:
		static void Init();
		static RawModel CreateQuad();
	};

} }