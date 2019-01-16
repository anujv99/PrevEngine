#include "pch.h"
#include "openglwindowsinitialization.h"

#include <glad/glad_wgl.h>
#include <glad/glad.h>

namespace prev { namespace windows { namespace opengl {
	OpenGLAPI::~OpenGLAPI() {
		Delete();
	}
	void OpenGLAPI::Init(HWND windowHandle, unsigned int windowWidth, unsigned int windowHeight) {
		m_PFD = {
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA,
				32,
				8, 0, 8, 8, 8, 16,
				8, 24,
				0,
				0, 0, 0, 0,
				24,
				8,
				0,
				PFD_MAIN_PLANE,
				0,
				0, 0, 0
		};

		m_HandleToDeviceContext = GetDC(windowHandle);

		int letWindowsChooseThisPixelFormat;
		letWindowsChooseThisPixelFormat = ChoosePixelFormat(m_HandleToDeviceContext, &m_PFD);
		if(!SetPixelFormat(m_HandleToDeviceContext, letWindowsChooseThisPixelFormat, &m_PFD)) {
			PV_CORE_CRITICAL("Unable to set pixel format opengl");
			isReady = false;
			return;
		}

		m_OpenGLRenderingContext = wglCreateContext(m_HandleToDeviceContext);
		if(!wglMakeCurrent(m_HandleToDeviceContext, m_OpenGLRenderingContext)) {
			PV_CORE_CRITICAL("Unable to create context opengl");
			isReady = false;
			return;
		}

		isReady = LoadGladOpenGL([](const char * func)->void* {
			void * p = (void *)wglGetProcAddress(func);
			if(p == 0 ||
				(p == (void*)0x1) ||
			    (p == (void*)0x2) ||
			    (p == (void*)0x3) ||
			    (p == (void*)-1)) {
				HMODULE module = LoadLibraryA("opengl32.dll");
				p = (void *)GetProcAddress(module, func);
			}
			return p;
		});

		isReady = LoadwglGladOpenGL(m_HandleToDeviceContext, [](const char * func)->void* {
			void * p = (void *)wglGetProcAddress(func);
			return p;
		});

		if(isReady) {
			m_Desc = (char *)glGetString(GL_VERSION);
			PV_CORE_INFO("OpenGL Version : %s", m_Desc.c_str());
		} else {
			PV_CORE_CRITICAL("Unable to initialize OpenGL");
			return;
		}

	}

	void OpenGLAPI::Update() {
		SwapBuffers(m_HandleToDeviceContext);
	}

	void OpenGLAPI::Delete() {
		wglDeleteContext(m_OpenGLRenderingContext);
	}

	void OpenGLAPI::SetVsync(bool enbaled) {
		if(enbaled)
			wglSwapIntervalEXT(1);
		else
			wglSwapIntervalEXT(0);
	}

	bool OpenGLAPI::LoadGladOpenGL(void *(*glProc)(const char *)) {
		if(!gladLoadGLLoader(glProc)) {
			PV_CORE_ERROR("Unable to initialize glad");
			return false;
		}
		PV_CORE_INFO("Glad successfully initialized");
		return true;
	}

	bool OpenGLAPI::LoadwglGladOpenGL(HDC hdc, void *(*glProc)(const char *)) {
		if(!gladLoadWGLLoader(glProc, hdc)) {
			PV_CORE_ERROR("Unable to initialize wglglad");
			return false;
		}
		PV_CORE_INFO("WGLGlad successfully initialized");
		return true;
	}

} } }
