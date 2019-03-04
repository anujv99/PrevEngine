#pragma once

#include <imgui.h>
#include "engine/layer.h"
#include "engine/window.h"

#include "engine/events/mouseevent.h"
#include "engine/events/applicationevent.h"
#include "engine/events/keyevent.h"
#include "collision/box2ddebuglayer.h"

namespace prev {

	class ImGuiWrapper {
		friend class ImGuiLayer;
	public:
		static ImGuiWrapper * Initialize();
		ImGuiWrapper() {}
		virtual ~ImGuiWrapper() {}
	protected:
		virtual void Init() = 0;
		virtual void NewFrame() = 0;
		virtual void Render() = 0;
		virtual void OnEvent(Event & event) = 0;
	};

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event &event);
	private:
		void ShowLogger();
		void ShowMainMenuBar();
		void ShowMiscPropsWindow();
	private:
		std::map<LogLevel, ImVec4> m_LogColors;
		ImGuiWrapper * m_API;
		b2Draw * m_B2DebugLayer = nullptr;
	
	};

}