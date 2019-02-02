#pragma once

#include <glad/glad.h>

namespace prev { namespace opengl {

	class Ibo {
		friend class OpenGLObjectsManager;
	private:
		Ibo();
		void Release() const;
	public:
		Ibo(const Ibo& oldVBO);
		~Ibo();
		void LoadData(GLsizeiptr size, const void * data) const;

		inline void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboID); }
		inline void UnBind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
	private:
		GLuint m_VboID = 0;
	};

} }