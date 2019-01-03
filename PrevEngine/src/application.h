#pragma once
#include <vld.h>
#include "engine/window.h"

#include "engine/events/applicationevent.h"
#include "essentials/input.h"

namespace prev {

	class Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event &event);
	private:
		bool OnWindowClose(WindowCloseEvent &e);

		std::shared_ptr<Window> m_Window;
		std::unique_ptr<Input> m_Input;
		bool m_Running = true;
	};

}