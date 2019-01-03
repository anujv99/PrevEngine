#pragma once
#include <vld.h>
#include "engine/window.h"

#include "engine/events/applicationevent.h"

namespace prev {

	class Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event &event);
	private:
		bool OnWindowClose(WindowCloseEvent &e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

}