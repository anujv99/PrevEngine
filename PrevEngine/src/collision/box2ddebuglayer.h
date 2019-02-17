#pragma once

#include "engine/layer.h"

namespace prev {

	class Box2DDebugWrapper {
		friend class Box2DDebugLayer;
	protected:
		static Box2DDebugWrapper * Initalize();
		virtual void OnUpdate() = 0;
		virtual b2Draw * Getb2Draw() = 0;
		virtual ~Box2DDebugWrapper() {}
		Box2DDebugWrapper() {}
	};
	
	class Box2DDebugLayer final : public Layer {
	public:
		Box2DDebugLayer();
		~Box2DDebugLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;

		b2Draw * Getb2Draw();
	private:
		Box2DDebugWrapper * m_DebugDraw;
	};

}