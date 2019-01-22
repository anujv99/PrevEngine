#include "pch.h"
#include "imguidirectxinit.h"

#ifdef PV_RENDERING_API_DIRECTX

#include <imgui.h>
#include "imgui_impl_dx11.h"
#include "../directx.h"
#include "application.h"

namespace prev {

	ImGuiWrapper * ImGuiWrapper::Initialize() {
		return new ImGuiDirectXGLInit();
	}

	ImGuiDirectXGLInit::ImGuiDirectXGLInit() : ImGuiWrapper() {}

	ImGuiDirectXGLInit::~ImGuiDirectXGLInit() {
		ImGui_ImplDX11_Shutdown();
	}

	void prev::ImGuiDirectXGLInit::Init() {
		auto api = *(DirectX*)(&Application::GetApplicationInstance()->GetGraphicsAPI());
		ImGui_ImplDX11_Init((ID3D11Device*)api.GetDirectXDevice(), (ID3D11DeviceContext*)api.GetDirectXDeviceContext());
	}

	void ImGuiDirectXGLInit::NewFrame() {
		ImGui_ImplDX11_NewFrame();
	}

	void ImGuiDirectXGLInit::Render(ImDrawData * drawData) {
		ImGui_ImplDX11_RenderDrawData(drawData);
	}

}


#endif