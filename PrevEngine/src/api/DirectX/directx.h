#pragma once

#ifdef PV_RENDERING_API_DIRECTX

#include <d3d11.h>
#include "../api.h"

namespace prev {

	class DirectX : public API {
		friend class ImGuiDirectXGLInit;
	public:
		DirectX(unsigned int windowWidth, unsigned int windowHeight);
		~DirectX();

		void OnUpdate() override;
	private:
		const ID3D11Device * GetDirectXDevice() const;
		const ID3D11DeviceContext * GetDirectXDeviceContext() const;
	private:
		void SetViewport() override;
	};

}

#endif