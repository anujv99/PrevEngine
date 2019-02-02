#include "pch.h"
#include "vbo.h"

namespace prev { namespace opengl {

	Vbo::Vbo() {
		glGenBuffers(1, &m_VboID);
	}

	Vbo::~Vbo() {}

	Vbo::Vbo(const Vbo& oldVBO) {
		m_VboID = oldVBO.m_VboID;
	}

	void Vbo::Release() const {
		glDeleteBuffers(1, &m_VboID);
	}

	void Vbo::LoadData(GLsizeiptr size, const void * data) const {
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

} }
