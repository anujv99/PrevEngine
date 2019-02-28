#pragma once

#include "api/instancedbuffer.h"
#include "api/OpenGL/openglrenderer.h"

namespace prev { namespace opengl {

	class Openglnstancing : public InstancedBuffer {
	public:
		Openglnstancing(unsigned long long maxDataSize);
		~Openglnstancing();
	private:
		virtual void AppendData(unsigned long long int dataSize, const void * data) override;
		virtual void ReplaceData(unsigned long long int dataOffet, unsigned long long dataSize, const void * data) override;
	public:
		RawModel m_Model;
		const Vbo * m_InstancedVBO;
	};

} }