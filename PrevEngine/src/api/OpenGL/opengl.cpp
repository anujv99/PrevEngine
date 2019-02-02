#include "pch.h"
#include "opengl.h"
#ifdef PV_RENDERING_API_OPENGL

#include "openglobjectsmanager.h"
#include "math/math.h"

namespace prev {

	API * API::Create(unsigned int windowWidth, unsigned int windowHeight) {
		return new opengl::OpenGL(windowWidth, windowHeight);
	}

	namespace opengl {

		const Shader * shader;
		const Vbo * vbo;
		const Ibo * ibo;
		const Vao * vao;
		static int pm = 0;
		static int mm = 0;
		const glm::mat4 * pmp = nullptr;

		OpenGL::OpenGL(unsigned int windowWidth, unsigned int windowHeight) : API(windowWidth, windowHeight) {
			SetViewport();
			shader = ShaderManager::LoadShader("tempShader", "C:/users/preve/desktop/shader.vert", "C:/users/preve/desktop/shader.frag");
			pm = shader->GetUniformLocation("projection");
			mm = shader->GetUniformLocation("model");
			vbo = OpenGLObjectsManager::CreateVBO();
			vao = OpenGLObjectsManager::CreateVAO();
			ibo = OpenGLObjectsManager::CreateIBO();
			pmp = Math::GetProjectionMatrixPointer();
			float vertices[] = {
				-0.5f, 0.5f,
				-0.5f, -0.5f,
				0.5f, -0.5f,
				0.5f, 0.5f
			};
			int indices[] = {
				0, 1, 2,
				0, 2, 3
			};
			vbo->LoadData(sizeof(vertices), vertices);
			ibo->LoadData(sizeof(indices), indices);
			vao->LoadVbo(vbo, 0, 2, GL_FLOAT, 0, 0);
			vao->EnableAttribArray(0);
			vao->UseIndexBuffer(ibo);
			vao->UnBind();
		}

		OpenGL::~OpenGL() {
			OpenGLObjectsManager::ReleaseEverything();
		}

		void OpenGL::OnUpdate() {
			glClearColor(0, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shader->UseShader();
			shader->LoadUniform(*pmp, pm);
			vao->Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		void OpenGL::OnEvent(Event &e) {
			EventDispatcher dispatcher(e);
			dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OpenGL::WindowSizeCallback));
		}

		void OpenGL::SetViewport() {
			glViewport(0, 0, m_WindowWidth, m_WindowHeight);
		}

	}
}

#endif