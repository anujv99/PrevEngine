#pragma once

#include "imgui/imguilayer.h"

namespace prev {

	class ImGuiOpenGLInit : public ImGuiWrapper {
	public:
		ImGuiOpenGLInit();
		~ImGuiOpenGLInit();
		void Init() override;
		void NewFrame() override;
		void Render(ImDrawData * drawData) override;
	};

}