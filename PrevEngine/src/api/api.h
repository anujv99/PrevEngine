#pragma once

#include "pch.h"

#include "engine/events/applicationevent.h"

namespace prev {

	class API {
	public:
		API(unsigned int windowWidth, unsigned int windowHeight) : 
			m_WindowWidth(windowWidth), m_WindowHeight(windowHeight) {}
		virtual ~API() {}

		virtual void OnUpdate() = 0;
		virtual void OnEvent(EventDispatcher &dispatcher) {}
		//Defined in specific api eg (opengl, directx)
		static API * Create(unsigned int windowWidth, unsigned int windowHeight);
	protected:
		virtual void SetViewport() = 0;
		std::string m_ApiType;
		unsigned int m_WindowWidth = 0, m_WindowHeight = 0;

		bool WindowSizeCallback(WindowResizeEvent &e) {
			m_WindowWidth = e.GetWidth();
			m_WindowHeight = e.GetHeight();
			SetViewport();
			return true;
		}
	};

} 
