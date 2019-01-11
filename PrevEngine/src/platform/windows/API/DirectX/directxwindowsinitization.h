#pragma once

#include "../api.h"
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <wrl/client.h>

#define COM_PTR Microsoft::WRL::ComPtr

namespace prev { namespace windows { namespace directx {

	class DirectX : public GraphicsAPI {
	public:
		DirectX() : GraphicsAPI() {}
		~DirectX();
		void Init(HWND windowHandle, unsigned int windowWidth, unsigned int windowHeight) override;
		void Update() override;
		void Delete() override;
		void SetVsync(bool enabled) override;
	private:
		bool InitializeDirectX(HWND hWnd, unsigned int windowWidth, unsigned int windowHeight);
	private:
		COM_PTR<ID3D11Device>			m_Device;
		COM_PTR<ID3D11DeviceContext>	m_DeviceContext;
		COM_PTR<IDXGISwapChain>			m_SwapChain;
		COM_PTR<ID3D11RenderTargetView> m_RenderTargetView;
	};

} } }
