#include "pch.h"
#include "directxwindowsinitization.h"
#include "adapterreader.h"

namespace prev { namespace windows { namespace directx {

	

	DirectX::~DirectX() {

	}

	void DirectX::Init(HWND windowHandle, unsigned int windowWidth, unsigned int windowHeight) {
		bool status = InitializeDirectX(windowHandle, windowWidth, windowHeight);
	}

	void DirectX::Update() {

	}

	void DirectX::Delete() {

	}

	void DirectX::SetVsync(bool enabled) {

	}

	bool DirectX::InitializeDirectX(HWND hWnd, unsigned int windowWidth, unsigned int windowHeight) {
		auto adapters = AdapterReader::GetAdapterData();
		return true;
	}

} } }