#pragma once

#include "openglobjectsmanager.h"

namespace prev { namespace opengl {

	struct RawModel {
		const Vao * m_Vao;
		unsigned int m_VertexCount;
	};

	class OpenGLRenderer {
		friend class OpenGL;
	private:
		static void Init();
		static void CreateQuad();
	};

} }