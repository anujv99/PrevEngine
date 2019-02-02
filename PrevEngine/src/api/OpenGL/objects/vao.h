#pragma once

#include "vbo.h"
#include "ibo.h"

namespace prev { namespace opengl {

	class Vao {
		friend class OpenGLObjectsManager;
	private:
		Vao();
		void Release() const;
	public:
		Vao(const Vao &oldVao);
		~Vao();
		void LoadVbo(const Vbo* vbo, GLuint index, GLint coordinateSize, GLenum type, GLsizei stride, const void* dataOffset) const;
		void UseIndexBuffer(const Ibo * indexBuffer) const;

		inline void Bind() const { glBindVertexArray(m_VaoID); }
		inline void UnBind() const { glBindVertexArray(0); }
		inline void EnableAttribArray(unsigned int attribNumber = 0) const { for (unsigned int i = 0; i <= attribNumber; i++) glEnableVertexAttribArray(i); }
		inline void DisableAttribArray(unsigned int attribNumber = 0) const { for (unsigned int i = 0; i <= attribNumber; i++) glDisableVertexAttribArray(i); }
	private:
		GLuint m_VaoID = 0;
	};

} }