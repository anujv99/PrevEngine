#include "pch.h"
#include "adapterreader.h"

namespace prev { namespace windows {

	std::vector<AdapterData> AdapterReader::m_Adapters;

	std::vector<AdapterData> AdapterReader::GetAdapterData() {
		if (m_Adapters.size() > 0)
			return m_Adapters;

		Microsoft::WRL::ComPtr<IDXGIFactory> factory;

		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));
		if (FAILED(hr)) {
			PV_ASSERT(-1, "Unable to create DXGIFactory for enumerating adapters");
		}

		IDXGIAdapter * adapter;
		UINT index = 0;
		while (SUCCEEDED(factory->EnumAdapters(index, &adapter))) {
			m_Adapters.push_back(AdapterData(adapter));
			index++;
		}
		return m_Adapters;
	}

	AdapterData::AdapterData(IDXGIAdapter * adapter) {
		m_Adapter = adapter;
		HRESULT hr = m_Adapter->GetDesc(&this->m_Description);
		if (FAILED(hr)) {
			PV_CORE_ERROR("Failed to get description of IDXGIAdapter");
		}
	}

} }
