#pragma once

#ifdef PV_RENDERING_API_OPENGL

#include "imgui/imguilayer.h"

namespace prev { namespace opengl {

	class ImGuiOpenGLInit : public ImGuiWrapper {
	public:
		ImGuiOpenGLInit();
		~ImGuiOpenGLInit();
		void Init() override;
		void NewFrame() override;
		void Render(ImDrawData * drawData) override;
	};

} }

#endif