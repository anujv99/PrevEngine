#include "pch.h"
#include "openglrenderer.h"

#include "api/baserenderer.h"
#include "math/math.h"

namespace prev {

	static opengl::RawModel m_QuadModel;

	void BaseRenderer::RenderQuad(const Shader * shader, glm::vec2 & position, glm::vec2 & scale, float rotation) {
		glm::mat4 model;
		Math::CreateModelMatrix(model, position, scale, rotation);
		shader->UseShader();
		int modelLocation = shader->GetUniformLocation("model");
		int projectionLocation = shader->GetUniformLocation("projection");
		shader->LoadUniform(model, modelLocation);
		shader->LoadUniform(Math::GetProjectionMatrix(), projectionLocation);
		m_QuadModel.m_Vao->Bind();
		m_QuadModel.m_Vao->EnableAttribArray(0);
		glDrawElements(GL_TRIANGLES, m_QuadModel.m_VertexCount, GL_UNSIGNED_INT, NULL);
	}

	namespace opengl {


		void OpenGLRenderer::Init() {
			CreateQuad();
		}

		void OpenGLRenderer::CreateQuad() {
			const Vao * vao = OpenGLObjectsManager::CreateVAO();
			const Vbo * vbo = OpenGLObjectsManager::CreateVBO();
			const Ibo * ibo = OpenGLObjectsManager::CreateIBO();

			const float vertices[] = {
				-0.5f, 0.5f,
				-0.5f, -0.5f,
				0.5f, -0.5f,
				0.5f, 0.5f
			};

			const int indices[] = {
				0, 1, 2,
				0, 2, 3
			};

			vbo->LoadData(sizeof(vertices), (void *)vertices);
			ibo->LoadData(sizeof(indices), (void *)indices);
			vao->LoadVbo(vbo, 0, 2, GL_FLOAT, 0, nullptr);
			vao->UseIndexBuffer(ibo);

			m_QuadModel = { vao, 6 };
		}

	}

}