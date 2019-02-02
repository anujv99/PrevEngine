#pragma once

#include <imgui.h>
#include "engine/layer.h"
#include "engine/window.h"

#include "engine/events/mouseevent.h"
#include "engine/events/applicationevent.h"
#include "engine/events/keyevent.h"

namespace prev {

	class ImGuiWrapper {
		friend class ImGuiLayer;
	public:
		static ImGuiWrapper * Initialize();
		ImGuiWrapper() {}
		virtual ~ImGuiWrapper() {}
	protected:
		virtual void Init() = 0;
		virtual void NewFrame() = 0;
		virtual void Render(ImDrawData * drawData) = 0;
	};

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event &event);
	private:
		void ShowLogger();
		void ShowMainMenuBar();
	private:
		std::map<ImGuiMouseCursor, CursorType> m_ImGuiMouseCursorMap;
		std::map<LogLevel, ImVec4> m_LogColors;
		ImGuiWrapper * m_API;
		unsigned int m_WindowWidth, m_WindowHeight;
		void UpdateMouseCursor();
	private:
		bool MouseMoved(MouseMovedEvent &e);
		bool MouseButtonPressed(MouseButtonPressedEvent &e);
		bool MouseButtonReleased(MouseButtonReleasedEvent &e);
		bool MouseScrolled(MouseScrolledEvent &e);
		bool WindowResized(WindowResizeEvent &e);
		bool KeyPressed(KeyPressedEvent &e);
		bool KeyReleased(KeyReleasedEvent &e);
		bool CharacterInputEvent(CharacterEvent &e);
	};

}