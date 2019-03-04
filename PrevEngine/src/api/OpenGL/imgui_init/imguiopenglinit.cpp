#include "pch.h"
#include "imguiopenglinit.h"

#ifdef PV_RENDERING_API_OPENGL

#include <glad/glad.h>
#include <imgui.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_opengl3.cpp"

#include "application.h"

namespace prev {

	ImGuiWrapper * ImGuiWrapper::Initialize() {
		return new opengl::ImGuiOpenGLInit();
	}

	namespace opengl {

		ImGuiOpenGLInit::ImGuiOpenGLInit() : ImGuiWrapper() { }

		ImGuiOpenGLInit::~ImGuiOpenGLInit() {
			ImGui_ImplOpenGL3_Shutdown();
		}

		void ImGuiOpenGLInit::Init() {
			ImGui_ImplOpenGL3_Init("#version 460 core");
		}

		void ImGuiOpenGLInit::NewFrame() {
			ImGui_ImplOpenGL3_NewFrame();
		}

		void ImGuiOpenGLInit::Render(ImDrawData* drawData) {
			ImGui::UpdatePlatformWindows();
			ImGui_ImplOpenGL3_RenderDrawData(drawData);
		}

	}

}


#endif