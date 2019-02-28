#pragma once

#include <glad/glad.h>

namespace prev { namespace opengl {

	class Vbo {
		friend class OpenGLObjectsManager;
	private:
		Vbo();
		void Release() const;
	public:
		Vbo(const Vbo &oldVBO);
		~Vbo();
		void LoadData(GLsizeiptr size, const void* data) const;

		inline void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_VboID); }
		inline void UnBind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

		inline GLuint GetVboID() const { return m_VboID; }
	private:
		GLuint m_VboID;
	};

} }
