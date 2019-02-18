#pragma once

#include "engine/window.h"

#define MAX_KEYBOARD_KEYS 500

#include "essentials/keycodes.h"
#include "essentials/mousebuttoncodes.h"

namespace prev {

	class Input final {
		friend class Application;
	public:
		Input(std::shared_ptr<Window> win);
		~Input();

		static bool IsKeyDown(int keyCode);
		static bool IsKeyPressed(int keyCode);
		static bool IsKeyUp(int keyCode);
		static bool IsKeyReleased(int keyCode);
	private:
		void OnUpdate();
		void OnEvent(Event &e);
		bool MouseMovedCallback(MouseMovedEvent &e);
		bool MouseScrolledCallback(MouseScrolledEvent &e);
	private:
		static std::shared_ptr<Window> m_Window;
		static bool m_KeyboardKeys[MAX_KEYBOARD_KEYS];
		static int m_MouseXPos, m_MouseYPos;
		static int m_MouseDeltaXPos, m_MouseDeltaYPos;
		static int m_MouseScrollDelta;
	};

}