#include "pch.h"
#include "directxwindowsinitization.h"

#include "adapterreader.h"
#include <comdef.h>

namespace prev { namespace windows { namespace directx {

	DirectX::~DirectX() {

	}

	void DirectX::Init(HWND windowHandle, unsigned int windowWidth, unsigned int windowHeight) {
		bool status = InitializeDirectX(windowHandle, windowWidth, windowHeight);
		isReady = status;
	}

	void DirectX::Update() {
		static float bgColor[] = {0, 0, 1, 1 };
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), bgColor);
		m_SwapChain->Present(0, NULL);
	}

	void DirectX::Delete() {

	}

	void DirectX::SetVsync(bool enabled) {

	}

	bool DirectX::InitializeDirectX(HWND hWnd, unsigned int windowWidth, unsigned int windowHeight) {
		auto adapters = AdapterReader::GetAdapterData();

		if (adapters.size() < 1) {
			PV_CORE_CRITICAL("No DXGI Adapters found");
			return false;
		}

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		D3D_FEATURE_LEVEL selectedLevel;

		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(scd));

		scd.BufferDesc.Width					= windowWidth;
		scd.BufferDesc.Height					= windowHeight;
		scd.BufferDesc.RefreshRate.Numerator	= 60;
		scd.BufferDesc.RefreshRate.Denominator	= 1;
		scd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
		scd.SampleDesc.Count					= 1;
		scd.SampleDesc.Quality					= 0;
		scd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount							= 1;
		scd.OutputWindow						= hWnd;
		scd.Windowed							= TRUE;
		scd.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		
		_bstr_t desc(adapters[0].m_Description.Description);
		const char * desc_string = desc;
		PV_CORE_TRACE("Selected Video Adapter : %s", desc_string);

		HRESULT hr = D3D11CreateDeviceAndSwapChain(adapters[0].m_Adapter,
									  D3D_DRIVER_TYPE_UNKNOWN,
									  NULL,
									  NULL,
									  featureLevels,
									  ARRAYSIZE(featureLevels),
									  D3D11_SDK_VERSION,
									  &scd,
									  m_SwapChain.GetAddressOf(),
									  m_Device.GetAddressOf(),
									  &selectedLevel,
									  m_DeviceContext.GetAddressOf());

		if (FAILED(hr)) {
			PV_CORE_ERROR("Unable to create D3D11 device and swap chain");
			return false;
		}

		switch (selectedLevel) {
		case D3D_FEATURE_LEVEL_9_1:
		{
			PV_CORE_TRACE("Selected Feature Level : D3D_FEATURE_LEVEL_9_1");
		} break;
		case D3D_FEATURE_LEVEL_9_2:
		{
			PV_CORE_TRACE("Selected Feature Level : D3D_FEATURE_LEVEL_9_2");
		} break;
		case D3D_FEATURE_LEVEL_9_3:
		{
			PV_CORE_TRACE("Selected Feature Level : D3D_FEATURE_LEVEL_9_3");
		} break;
		case D3D_FEATURE_LEVEL_10_0:
		{
			PV_CORE_TRACE("Selected Feature Level : D3D_FEATURE_LEVEL_10_0");
		} break;
		case D3D_FEATURE_LEVEL_10_1:
		{
			PV_CORE_TRACE("Selected Feature Level : D3D_FEATURE_LEVEL_10_1");
		} break;
		case D3D_FEATURE_LEVEL_11_0:
		{
			PV_CORE_TRACE("Selected Feature Level : D3D_FEATURE_LEVEL_11_0");
		} break;
		case D3D_FEATURE_LEVEL_11_1:
		{
			PV_CORE_TRACE("Selected Feature Level : D3D_FEATURE_LEVEL_11_1");
		} break;
		case D3D_FEATURE_LEVEL_12_0:
		{
			PV_CORE_TRACE("Selected Feature Level : D3D_FEATURE_LEVEL_12_0");
		} break;
		case D3D_FEATURE_LEVEL_12_1:
		{
			PV_CORE_TRACE("Selected Feature Level : D3D_FEATURE_LEVEL_12_1");
		} break;
		default:
		{
			PV_CORE_TRACE("Selected Feature Level not found");
		} break;
		}

		COM_PTR<#ifdef PV_RENDERING_API_DIRECTXkBuffer;
		hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		if (FAILED(hr)) {
			PV_CORE_ERROR("Unable to get backbuffer");
			return false;
		}

		hr = m_Device->CreateRenderTargetView(backBuffer.Get(), NULL, m_RenderTargetView.GetAddressOf());
		if (FAILED(hr)) {
			PV_CORE_ERROR("Unable to create render target view");
			return false;
		}

		m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), NULL);

		return true;
	}

} } }
