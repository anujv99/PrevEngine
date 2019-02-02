#include "pch.h"
#include "ibo.h"

namespace prev { namespace opengl {

	Ibo::Ibo() {
		glGenBuffers(1, &m_VboID);
	}

	void Ibo::Release() const {
		glDeleteBuffers(1, &m_VboID);
	}

	Ibo::Ibo(const Ibo& oldVBO) {
		m_VboID = oldVBO.m_VboID;
	}

	Ibo::~Ibo() {}

	void Ibo::LoadData(GLsizeiptr size, const void* data) const {
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

} }