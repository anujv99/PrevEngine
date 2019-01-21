#include "pch.h"
#include "imguiopenglinit.h"

#include <glad/glad.h>
#include <imgui.h>

#include "imgui_impl_opengl3.h"

namespace prev {

	ImGuiWrapper * ImGuiWrapper::Initialize() {
		return new ImGuiOpenGLInit();
	}

	ImGuiOpenGLInit::ImGuiOpenGLInit() : ImGuiWrapper() {}

	ImGuiOpenGLInit::~ImGuiOpenGLInit() {
		ImGui_ImplOpenGL3_Shutdown();
	}

	void prev::ImGuiOpenGLInit::Init() {
		ImGui_ImplOpenGL3_Init("#version 460 core");
	}

	void ImGuiOpenGLInit::NewFrame() {
		ImGui_ImplOpenGL3_NewFrame();
	}

	void ImGuiOpenGLInit::Render(ImDrawData * drawData) {
		ImGui_ImplOpenGL3_RenderDrawData(drawData);
	}

}
