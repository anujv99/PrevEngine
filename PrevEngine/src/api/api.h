#pragma once

#include "pch.h"

#include "shadermanager.h"
#include "texturemanager.h"
#include "engine/layer.h"

namespace prev {

	class API : public Layer {
	public:
		virtual ~API() {
			ShaderManager::ReleaseShaders();
			TextureManager::ReleaseTextures();
		}

		virtual void OnUpdate() override = 0;
		virtual void OnEvent(Event &e) override {}
		//Defined in specific api eg (opengl, directx => Currently not supported)
		static API * Create();
	protected:
		API(unsigned int windowWidth, unsigned int windowHeight) : 
			m_WindowWidth(windowWidth), m_WindowHeight(windowHeight), Layer("OpenGLLayer") {}
		virtual void SetViewport() = 0;
		std::string m_ApiType;
		unsigned int m_WindowWidth = 0, m_WindowHeight = 0;

		bool WindowSizeCallback(WindowResizeEvent &e) {
			m_WindowWidth = e.GetWidth();
			m_WindowHeight = e.GetHeight();
			SetViewport();
			return false;
		}
	};

} 
