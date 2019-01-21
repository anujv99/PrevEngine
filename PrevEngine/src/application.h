#pragma once
#include "engine/window.h"
#include "api/api.h"

#include "engine/layerstack.h"
#include "engine/events/applicationevent.h"
#include "essentials/input.h"

namespace prev {

	class Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event &event);

		void PushLayer(Layer * layer);
		void PushOverlay(Layer * layer);

		Window &GetWindow() const { return *m_Window; }

		static Application * GetApplicationInstance();
	private:
		bool OnWindowClose(WindowCloseEvent &e);

		std::shared_ptr<Window> m_Window;
		std::shared_ptr<Input> m_Input;
		std::unique_ptr<API> m_GraphicsAPI;

		LayerStack m_LayerStack;

		bool m_Running = true;
	};

}