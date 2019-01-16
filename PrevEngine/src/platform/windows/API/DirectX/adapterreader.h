#pragma once

#include "directxwindowsinitization.h"

namespace prev { namespace windows { namespace directx {

	class AdapterData {
	public:
		AdapterData(IDXGIAdapter * adapter);
		IDXGIAdapter * m_Adapter = nullptr;
		DXGI_ADAPTER_DESC m_Description;
	};

	class AdapterReader {
	public:
		static std::vector<AdapterData> GetAdapterData();
	private:
		static std::vector<AdapterData> m_Adapters;
	};

} } }
