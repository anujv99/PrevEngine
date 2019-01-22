#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include "../graphicsapi.h"

namespace prev { namespace windows {

	struct DirectXData {
		Microsoft::WRL::ComPtr<ID3D11Device>			m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_DeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain>			m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_RenderTargetView;
	};

	class WindowsDirectX : public GraphicsAPI {
	public:
		WindowsDirectX() : GraphicsAPI() {}
		~WindowsDirectX();
		void Init(HWND windowHandle, unsigned int windowWidth, unsigned int windowHeight) override;
		void Update() override;
		void Delete() override;
		void SetVsync(bool enabled) override;
		const DirectXData* GetDirectXData() const { return &m_Data; }
	private:
		bool InitializeDirectX(HWND hWnd, unsigned int windowWidth, unsigned int windowHeight);
	private:
		DirectXData m_Data;
		bool vSync;
	};

} }
