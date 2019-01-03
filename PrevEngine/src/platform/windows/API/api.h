#pragma once

#include "pch.h"

namespace prev { namespace windows {

	class GraphicsAPI {
	public:
		GraphicsAPI() {}
		virtual ~GraphicsAPI() {}
		inline bool IsAPIReady() const { return isReady; }

		virtual void Init(HWND windowHandle) = 0;
		virtual void Update() = 0;
		virtual void Delete() = 0;

		virtual void SetVsync(bool enbaled) = 0;
	protected:
		bool isReady = true;
		std::string m_Desc;
	};

} }
