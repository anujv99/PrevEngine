#include "pch.h"
#include "vao.h"

namespace prev { namespace opengl {

	Vao::Vao() {
		glGenVertexArrays(1, &m_VaoID);
	}

	Vao::Vao(const Vao& oldVao) {
		m_VaoID = oldVao.m_VaoID;
	}

	Vao::~Vao() {}

	void Vao::Release() const {
		glDeleteVertexArrays(1, &m_VaoID);
	}

	void Vao::LoadVbo(const Vbo * vbo, GLuint index, GLint coordinateSize, GLenum type, GLsizei stride, const void * dataOffset) const {
		Bind();
		vbo->Bind();
		glVertexAttribPointer(index, coordinateSize, type, GL_FALSE, stride, dataOffset);
	}

	void Vao::UseIndexBuffer(const Ibo * indexBuffer) const {
		Bind();
		indexBuffer->Bind();
	}

} }