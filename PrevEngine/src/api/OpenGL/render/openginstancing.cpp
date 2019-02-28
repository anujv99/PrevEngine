#include "pch.h"
#include "openginstancing.h"

namespace prev {

	InstancedBuffer * InstancedBuffer::Create(unsigned long long maxDataSize) {
		return new opengl::Openglnstancing(maxDataSize);
	}

	namespace opengl {

		Openglnstancing::Openglnstancing(unsigned long long maxDataSize) {
			m_MaxDataSize = maxDataSize;
			m_Model = OpenGLRenderer::CreateQuad();
			m_InstancedVBO = OpenGLObjectsManager::CreateVBO();

			m_Model.m_Vao->LoadVbo(m_InstancedVBO, 2, m_CoordinateSize, GL_FLOAT, 0, (void *)0);
			m_InstancedVBO->Bind();
			glBufferData(GL_ARRAY_BUFFER, m_MaxDataSize, nullptr, GL_DYNAMIC_DRAW);
			m_InstancedVBO->UnBind();
			m_Model.m_Vao->Bind();
			glVertexAttribDivisor(2, m_UpdateInterval);
			m_Model.m_Vao->UnBind();

		}

		Openglnstancing::~Openglnstancing() {

		}

		void Openglnstancing::AppendData(unsigned long long int dataSize, const void * data) {
			m_InstancedVBO->Bind();
			glBufferSubData(GL_ARRAY_BUFFER, m_DataSize, dataSize, data);
			m_DataSize += dataSize;
		}

		void Openglnstancing::ReplaceData(unsigned long long int dataOffet, unsigned long long dataSize, const void * data) {
			m_InstancedVBO->Bind();
			glBufferSubData(GL_ARRAY_BUFFER, dataOffet, dataSize, data);
			if (dataOffet + dataSize > m_DataSize)
				m_DataSize = dataOffet + dataSize;
		}

	}

}