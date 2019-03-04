#pragma once

#include "engine/window.h"

#include "imgui/imguilayer.h"

namespace prev { namespace windows {

	class ImGuiOpenGLInit : public ImGuiWrapper {
	public:
		ImGuiOpenGLInit();
		~ImGuiOpenGLInit();
		void Init() override;
		void NewFrame() override;
		void Render() override;
		virtual void OnEvent(Event & event) override;
		void UpdateMouseCursor();
	private:
		unsigned int m_WindowWidth, m_WindowHeight;
		std::map<ImGuiMouseCursor, CursorType> m_ImGuiMouseCursorMap;
	private:
		bool MouseMoved(MouseMovedEvent & e);
		bool MouseButtonPressed(MouseButtonPressedEvent & e);
		bool MouseButtonReleased(MouseButtonReleasedEvent & e);
		bool MouseScrolled(MouseScrolledEvent & e);
		bool WindowResized(WindowResizeEvent & e);
		bool KeyPressed(KeyPressedEvent & e);
		bool KeyReleased(KeyReleasedEvent & e);
		bool CharacterInputEvent(CharacterEvent & e);
	};

} }