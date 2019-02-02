#pragma once

#include "objects/vao.h"

namespace prev { namespace opengl {

	class OpenGLObjectsManager {
	public:
		static const Vao * CreateVAO();
		static const Vbo * CreateVBO();
		static const Ibo * CreateIBO();
		static void ReleaseEverything();
	private:
		static std::list<Vao> m_Vaos;
		static std::list<Vbo> m_Vbos;
		static std::list<Ibo> m_Ibos;
	};

} }