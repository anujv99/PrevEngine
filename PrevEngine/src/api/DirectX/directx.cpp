#include "pch.h"
#include "directx.h"

#ifdef PV_RENDERING_API_DIRECTX

#include <DirectXMath.h>
#include "application.h"

#include "platform/windows/API/DirectX/directxwindowsinitization.h"
#include "platform/windows/windowswindow.h"

namespace prev {

	static const windows::DirectXData * m_Data;

	API * API::Create(unsigned int windowWidth, unsigned int windowHeight) {
		return new DirectX(windowWidth, windowHeight);
	}

	DirectX::DirectX(unsigned int windowWidth, unsigned int windowHeight) : API(windowWidth, windowHeight) {
		windows::WindowsWindow * window = (windows::WindowsWindow*)(&(Application::GetApplicationInstance()->GetWindow()));
		windows::WindowsDirectX * api = (windows::WindowsDirectX *)window->GetGraphicsAPI();
		m_Data = api->GetDirectXData();
	}

	DirectX::~DirectX() {
	}

	void DirectX::OnUpdate() {
		static float bgColor[] = {0, 0, 1, 1};
		m_Data->m_DeviceContext->ClearRenderTargetView(m_Data->m_RenderTargetView.Get(), bgColor);
	}

	const ID3D11Device * DirectX::GetDirectXDevice() const {
		return m_Data->m_Device.Get();
	}

	const ID3D11DeviceContext * DirectX::GetDirectXDeviceContext() const {
		return m_Data->m_DeviceContext.Get();
	}

	void DirectX::SetViewport() {
	}

}

#endif