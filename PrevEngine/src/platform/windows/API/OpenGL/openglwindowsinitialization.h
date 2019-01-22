#pragma once

#include "../graphicsapi.h"

namespace prev { namespace windows {

	class OpenGLAPI : public GraphicsAPI {
	public:
		OpenGLAPI() : GraphicsAPI() {}
		~OpenGLAPI();
		void Init(HWND windowHandle, unsigned int windowWidth, unsigned int windowHeight) override;
		void Update() override;
		void Delete() override;
		void SetVsync(bool enbaled) override;
	private:
		bool LoadGladOpenGL(void * (*glProc)(const char *));
		bool LoadwglGladOpenGL(HDC hdc, void * (*glProc)(const char *));
	private:
		PIXELFORMATDESCRIPTOR m_PFD = {0};
		HDC m_HandleToDeviceContext;
		HGLRC m_OpenGLRenderingContext;
	};

} }
