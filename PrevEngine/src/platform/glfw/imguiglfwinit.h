#pragma once

#include "engine/window.h"
#include "imgui/imguilayer.h"

namespace prev { namespace glfw {

	class ImGuiGlfwInit : public ImGuiWrapper {
	public:
		ImGuiGlfwInit();
		~ImGuiGlfwInit();
	private:
		unsigned int m_WindowWidth, m_WindowHeight;
		std::map<ImGuiMouseCursor, CursorType> m_ImGuiMouseCursorMap;

		// Inherited via ImGuiWrapper
		virtual void Init() override;
		virtual void NewFrame() override;
		virtual void Render() override;
		virtual void OnEvent(Event & event) override;
	};

} }