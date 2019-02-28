#include "pch.h"
#include "openglrenderer.h"

#include "api/baserenderer.h"
#include "math/math.h"
#include "api/OpenGL/render/openginstancing.h"

namespace prev {

	static opengl::RawModel m_QuadModel;

	void BaseRenderer::RenderQuad(const Shader * shader, glm::vec2 & position, glm::vec2 & scale, float rotation) {
		glm::mat4 model;
		Math::CreateModelMatrix(model, position, scale, rotation);
		shader->LoadModelMatrix(model);
		shader->LoadProjectionMatrix(Math::GetProjectionMatrix());
		m_QuadModel.m_Vao->Bind();
		m_QuadModel.m_Vao->EnableAttribArray(1);
		glDrawElements(GL_TRIANGLES, m_QuadModel.m_VertexCount, GL_UNSIGNED_INT, NULL);
	}

	void BaseRenderer::RenderQuadInstanced(const InstancedBuffer * buffer) {
		opengl::Openglnstancing * openglBuffer = (opengl::Openglnstancing *)(buffer);
		openglBuffer->m_Model.m_Vao->Bind();
		openglBuffer->m_Model.m_Vao->EnableAttribArray(3);
		glDrawElementsInstanced(GL_TRIANGLES, openglBuffer->m_Model.m_VertexCount, GL_UNSIGNED_INT, NULL, openglBuffer->GetNumberOfInstances());
		//glDrawElements(GL_TRIANGLES, openglBuffer->m_Model.m_VertexCount, GL_UNSIGNED_INT, NULL);
	}

	namespace opengl {

		void OpenGLRenderer::Init() {
			m_QuadModel = CreateQuad();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		RawModel OpenGLRenderer::CreateQuad() {
			const Vao * vao = OpenGLObjectsManager::CreateVAO();
			const Vbo * vbo = OpenGLObjectsManager::CreateVBO();
			const Vbo * vbo2 = OpenGLObjectsManager::CreateVBO();
			const Ibo * ibo = OpenGLObjectsManager::CreateIBO();

			const float vertices[] = {
				-0.5f, 0.5f,
				-0.5f, -0.5f,
				0.5f, -0.5f,
				0.5f, 0.5f
			};

			const float textureCoords[] = { 
				0, 1,
				0, 0,
				1, 0,
				1, 1
			};

			const int indices[] = {
				0, 1, 2,
				0, 2, 3
			};

			vbo->LoadData(sizeof(vertices), (void *)vertices);
			vbo2->LoadData(sizeof(textureCoords), (void *)textureCoords);
			ibo->LoadData(sizeof(indices), (void *)indices);
			vao->LoadVbo(vbo, 0, 2, GL_FLOAT, 0, nullptr);
			vao->LoadVbo(vbo2, 1, 2, GL_FLOAT, 0, nullptr);
			vao->UseIndexBuffer(ibo);

			return { vao, 6 };
		}

	}

}