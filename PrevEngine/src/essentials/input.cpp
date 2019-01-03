#include "pch.h"
#include "input.h"

namespace prev {

	Input::Input(std::shared_ptr<Window> win) {
		m_Window = win;
	}

	Input::~Input() {
	}

	void Input::Update() {
		for(unsigned int i = 0; i < MAX_KEYBOARD_KEYS; i++) {
			m_KeyboardKeys[i] = IsKeyDown(i);
		}
	}

	bool Input::IsKeyDown(int keyCode) {
		return m_Window->IsKeyDown(keyCode);
	}

	bool Input::IsKeyPressed(int keyCode) {
		return (IsKeyDown(keyCode) && !m_KeyboardKeys[keyCode]);
	}

	bool Input::IsKeyUp(int keyCode) {
		return !m_Window->IsKeyDown(keyCode);
	}

	bool Input::IsKeyReleased(int keyCode) {
		return (IsKeyUp(keyCode) && m_KeyboardKeys[keyCode]);
	}

}