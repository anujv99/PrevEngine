#pragma once

#define MAX_KEYBOARD_KEYS 500

namespace prev {

	namespace keyboard {
		enum KeyboardKeys {

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
		Input();
		~Input();
	private:
		bool m_KeyboardKeys[MAX_KEYBOARD_KEYS];
	};

}