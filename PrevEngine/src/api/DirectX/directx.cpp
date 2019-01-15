#include "pch.h"
#include "directx.h"

#ifdef PV_RENDERING_API_DIRECTX

#include <DirectXMath.h>

namespace prev { 

	API * API::Create(unsigned int windowWidth, unsigned int windowHeight) {
		return new graphicsapi::directx::DirectX(windowWidth, windowHeight);
	}

	namespace graphicsapi { namespace directx {

		DirectX::DirectX(unsigned int windowWidth, unsigned int windowHeight) : API(windowWidth, windowHeight) {
		}

		DirectX::~DirectX() {
		}

		void DirectX::OnUpdate() {
		}

		void DirectX::SetViewport() {
		}

	} }

}

#endif