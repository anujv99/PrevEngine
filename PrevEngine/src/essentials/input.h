#pragma once

#include "engine/events/keyevent.h"
#include "engine/window.h"

#define MAX_KEYBOARD_KEYS 500

namespace prev {
	
	namespace keyboard {
		enum KeyboardKeys {
			PV_KEYBOARD_KEY_A,
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

	class Input {
	public:
		Input(std::shared_ptr<Window> win);
		~Input();

		void Update();

		bool IsKeyDown(int keyCode);
		bool IsKeyPressed(int keyCode);
		bool IsKeyUp(int keyCode);
		bool IsKeyReleased(int keyCode);
	private:
		std::shared_ptr<Window> m_Window;
	private:
		bool m_KeyboardKeys[MAX_KEYBOARD_KEYS];
	};

}