#pragma once

#include "event.h"

namespace prev {

	class KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keyCode) :
			m_KeyCode(keyCode) {}
		
		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keycode, bool repeatCount) :
			KeyEvent(keycode), m_Repeat(repeatCount) {}

		inline int IsRepeating() const { return m_Repeat; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_Repeat << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_Repeat;
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keycode) :
			KeyEvent(keycode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class CharacterEvent : public Event {
	public:
		CharacterEvent(unsigned char pressedChar) :
			m_PressedChar(pressedChar) {}

		inline char GetPressedChar() const { return m_PressedChar; }

		std::string ToString() const override {
			std::string str;
			str = m_PressedChar;
			return str;
		}

		EVENT_CLASS_TYPE(CharacterInput)
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	private:
		unsigned char m_PressedChar;
	};

}