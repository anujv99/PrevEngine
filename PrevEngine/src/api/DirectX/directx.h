#pragma once

#ifdef PV_RENDERING_API_DIRECTX

#ifndef PV_PLATFORM_WINDOWS
#error "DirectX Can only be used with windows (if using windows make sure to define PV_PLATFORM_WINDOWS)"
#endif

#include "../api.h"

namespace prev { namespace graphicsapi { namespace directx {

	class DirectX : public API {
	public:
		DirectX(unsigned int windowWidth, unsigned int windowHeight);
		~DirectX();

		void OnUpdate() override;
	private:
		void SetViewport() override;
	};

} } }

#endif