#pragma once

#include "engine/window.h"
#include "api/api.h"

#include "engine/layerstack.h"
#include "essentials/input.h"

namespace prev {

	class Application {
	public:
		Application(WindowProps winProps = WindowProps());
		virtual ~Application();

		void Run();

		void OnEvent(Event &event);

		void PushLayer(Layer * layer);
		void PushOverlay(Layer * layer);
		void PopLayer(Layer * layer);
		void PopOverlay(Layer * layer);
		//
		b2Draw * GetB2Draw();
		//
		Window &GetWindow() const { return *m_Window; }

		static Application * GetApplicationInstance();
	private:
		bool OnWindowClose(WindowCloseEvent &e);

		std::shared_ptr<Window> m_Window;
		std::shared_ptr<Input> m_Input;

		LayerStack m_LayerStack;

		bool m_Running = true;
	};

}
