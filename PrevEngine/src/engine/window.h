#pragma once

#include "pch.h"

#include "engine/events/event.h"

namespace prev {

	enum class CursorType {
		PV_ARROW,
		PV_TEXT_INPUT,
		PV_RESIZE_ALL,
		PV_RESIZE_EW,
		PV_RESIZE_NS,
		PV_RESIZE_NESW,
		PV_RESIZE_NWSE,
		PV_HAND
	};

	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		CursorType CType;

		WindowProps(const std::string &title = "Prev Engine", unsigned int width = 1280, unsigned int height = 720, CursorType ctype = CursorType::PV_ARROW) :
			Title(title), Width(width), Height(height), CType(ctype) {
		}

	};

	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn &callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual bool IsKeyDown(int keyCode) = 0;

		virtual void CreateOpenGLContext() = 0;

		virtual void ChangeCursor(CursorType type) = 0;

		virtual void * GetRawWindow() = 0;

		static Window * Create(const WindowProps &props = WindowProps());
		static std::string GetExePath();
	private:
		static std::string m_ExePath;
	};

}
