#include "pch.h"
#include "input.h"

namespace prev {

#define BIND_EVENT_FUNC(x) std::bind(&Input::x, this, std::placeholders::_1)

	bool Input::m_KeyboardKeys[MAX_KEYBOARD_KEYS]		= {0};
	int  Input::m_MouseXPos								= 0;
	int  Input::m_MouseYPos								= 0;
	int  Input::m_MouseDeltaXPos						= 0;
	int  Input::m_MouseDeltaYPos						= 0;
	int  Input::m_MouseScrollDelta						= 0;
	std::shared_ptr<Window> Input::m_Window;

	Input::Input(std::shared_ptr<Window> win) {
		m_Window = win;
	}

	Input::~Input() {
	}

	void Input::OnUpdate() {
		for (unsigned int i = 0; i < MAX_KEYBOARD_KEYS; i++) {
			m_KeyboardKeys[i] = IsKeyDown(i);
		}
		m_MouseScrollDelta = 0;
	}

	void Input::OnEvent(EventDispatcher &dispatcher) {
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUNC(MouseMovedCallback));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNC(MouseScrolledCallback));
	}

	bool Input::MouseMovedCallback(MouseMovedEvent & e) {
		int x = e.GetX(), y = e.GetY();
		m_MouseDeltaXPos = x - m_MouseXPos;
		m_MouseDeltaYPos = y - m_MouseYPos;
		m_MouseXPos = x;
		m_MouseYPos = y;
		return true;
	}

	bool Input::MouseScrolledCallback(MouseScrolledEvent &e) {
		m_MouseScrollDelta = (int)e.GetXOffset();
		return true;
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