#include "pch.h"
#include "imguiglfwinit.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_opengl3.cpp"

#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_glfw.cpp"

#include "application.h"

namespace prev {

	ImGuiWrapper * ImGuiWrapper::Initialize() {
		return new glfw::ImGuiGlfwInit();
	}

	namespace glfw {

		ImGuiGlfwInit::ImGuiGlfwInit() {
			m_WindowWidth = Application::GetApplicationInstance()->GetWindow().GetWidth();
			m_WindowHeight = Application::GetApplicationInstance()->GetWindow().GetHeight();
		}
	
		ImGuiGlfwInit::~ImGuiGlfwInit() {
		}

		void ImGuiGlfwInit::Init() {
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO & io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

			ImGui::StyleColorsDark();

			ImGuiStyle & style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Application::GetApplicationInstance()->GetWindow().GetRawWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 460 core");
		}

		void ImGuiGlfwInit::NewFrame() {
			/*ImGuiIO & io = ImGui::GetIO();
			io.DisplaySize = ImVec2((float)m_WindowWidth, (float)m_WindowHeight);
			io.DeltaTime = Timer::GetDeltaTime();*/

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void ImGuiGlfwInit::Render() {

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			ImGuiIO & io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				GLFWwindow * backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
		}

		void ImGuiGlfwInit::OnEvent(Event & event) {
		}

	} 
}