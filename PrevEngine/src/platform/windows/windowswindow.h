#pragma once

#include "API/api.h"
#include "engine/window.h"
#include "essentials/input.h"

namespace prev { namespace windows {

	class WindowsWindow : public Window {
		friend LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		friend class Input;
	public:
		WindowsWindow(const WindowProps &props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.width; }
		inline unsigned int GetHeight() const override { return m_Data.height; }

		inline void SetEventCallback(const EventCallbackFn &callback) override { m_Data.eventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		void CreateOpenGLContext() override;
		void CreateDirectXContext() override;

	private:
		bool IsKeyDown(int keyCode) override;
		virtual void ShutDown();
		std::map<int, int> m_KeyMap;
		std::map<int, int> m_ReverseKeyMap;
	private:
		struct WindowData {
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn eventCallback = nullptr;
		};
		struct OpenGLData {};
		struct DirectXData {

		};
		std::unique_ptr<GraphicsAPI> m_GraphicsAPI;
		WindowData m_Data;
		HWND hWnd;
		MSG msg;
		HGLRC ourOpenGLRenderingContext = 0;
	};

} }
