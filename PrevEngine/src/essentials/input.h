#pragma once

#include "engine/window.h"

#define MAX_KEYBOARD_KEYS 500

namespace prev {
	
	namespace keyboard {
		enum KeyboardKeys {
			PV_KEYBOARD_KEY_A = 0x100,
			PV_KEYBOARD_KEY_B,
			PV_KEYBOARD_KEY_C,
			PV_KEYBOARD_KEY_D,
			PV_KEYBOARD_KEY_E,
			PV_KEYBOARD_KEY_F,
			PV_KEYBOARD_KEY_G,
			PV_KEYBOARD_KEY_H,
			PV_KEYBOARD_KEY_I,
			PV_KEYBOARD_KEY_J,
			PV_KEYBOARD_KEY_K,
			PV_KEYBOARD_KEY_L,
			PV_KEYBOARD_KEY_M,
			PV_KEYBOARD_KEY_N,
			PV_KEYBOARD_KEY_O,
			PV_KEYBOARD_KEY_P,
			PV_KEYBOARD_KEY_Q,
			PV_KEYBOARD_KEY_R,
			PV_KEYBOARD_KEY_S,
			PV_KEYBOARD_KEY_T,
			PV_KEYBOARD_KEY_U,
			PV_KEYBOARD_KEY_V,
			PV_KEYBOARD_KEY_W,
			PV_KEYBOARD_KEY_X,
			PV_KEYBOARD_KEY_Y,
			PV_KEYBOARD_KEY_Z,

			PV_KEYBOARD_KEY_LEFT,
			PV_KEYBOARD_KEY_RIGHT,
			PV_KEYBOARD_KEY_UP,
			PV_KEYBOARD_KEY_DOWN,

			PV_KEYBOARD_KEY_ESCAPE,
			PV_KEYBOARD_KEY_TAB,
			PV_KEYBOARD_KEY_INSERT,
			PV_KEYBOARD_KEY_HOME,
			PV_KEYBOARD_KEY_PAGEUP,
			PV_KEYBOARD_KEY_PAGEDOWN,
			PV_KEYBOARD_KEY_END,
			PV_KEYBOARD_KEY_DELETE,
			PV_KEYBOARD_KEY_BACKSPACE,
			PV_KEYBOARD_KEY_SPACE,
			PV_KEYBOARD_KEY_ENTER,
		};
	}

	namespace mouse {
		enum MouseKeys {
			PV_MOUSE_BUTTON_LEFT	= 0x00,
			PV_MOUSE_BUTTON_RIGHT	= 0x01,
			PV_MOUSE_BUTTON_MIDDLE	= 0x02,
			PV_MOUSE_BUTTON_THUMB1	= 0x03,
			PV_MOUSE_BUTTON_THUMB2	= 0x04,
			PV_MOUSE_BUTTON_1		= 0x00,
			PV_MOUSE_BUTTON_2		= 0x01,
			PV_MOUSE_BUTTON_3		= 0x02,
			PV_MOUSE_BUTTON_4		= 0x03,
			PV_MOUSE_BUTTON_5		= 0x04,
		};
	}

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