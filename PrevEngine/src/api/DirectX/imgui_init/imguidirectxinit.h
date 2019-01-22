#pragma once

#ifdef PV_RENDERING_API_DIRECTX

#include "imgui/imguilayer.h"

namespace prev {

	class ImGuiDirectXGLInit : public ImGuiWrapper {
	public:
		ImGuiDirectXGLInit();
		~ImGuiDirectXGLInit();
		void Init() override;
		void NewFrame() override;
		void Render(ImDrawData * drawData) override;
	};

}

#endif