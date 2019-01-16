#pragma once

#include "engine/window.h"

namespace prev { namespace linux {

    class LinuxWindow : public Window {
    public:
        LinuxWindow(const WindowProps &props);
		virtual ~LinuxWindow();

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
        struct WindowData {
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn eventCallback = nullptr;
		};
        WindowData m_Data;
    };

} }