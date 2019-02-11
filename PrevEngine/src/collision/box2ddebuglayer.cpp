#include "pch.h"
#include "box2ddebuglayer.h"

namespace prev {

	Box2DDebugLayer::Box2DDebugLayer() : 
		Layer("Box2DDebugLayer") {
	}

	Box2DDebugLayer::~Box2DDebugLayer() {
		delete m_DebugDraw;
	}

	void Box2DDebugLayer::OnUpdate() {
		m_DebugDraw->OnUpdate();
	}

	b2Draw * Box2DDebugLayer::Getb2Draw() {
		return m_DebugDraw->Getb2Draw();
	}

	void Box2DDebugLayer::OnAttach() {
		m_DebugDraw = Box2DDebugWrapper::Initalize();
	}

	void Box2DDebugLayer::OnDetach() {
		delete m_DebugDraw;
	}

}