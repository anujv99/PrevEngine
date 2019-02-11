#include "pch.h"
#include "box2ddebugdrawopengl.h"

namespace prev {

	Box2DDebugWrapper * Box2DDebugWrapper::Initalize() {
		return new opengl::Box2DDebugDrawOpenGL();
	}

	namespace opengl {

		Box2DDebugDrawOpenGL::Box2DDebugDrawOpenGL() {
			g_debugDraw.Create();
		}

		Box2DDebugDrawOpenGL::~Box2DDebugDrawOpenGL() {
			g_debugDraw.Destroy();
		}

		void Box2DDebugDrawOpenGL::OnUpdate() {
			g_debugDraw.Flush();
		}

		b2Draw * Box2DDebugDrawOpenGL::Getb2Draw() {
			return &(g_debugDraw);
		}

	}

}

