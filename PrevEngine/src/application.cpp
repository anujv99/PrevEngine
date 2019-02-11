 #include "pch.h"
#include "application.h"
#include "essentials/timer.h"
#include "imgui/imguilayer.h"
#include "math/math.h"

#include "math/tiles.h"

#include <Box2D/Box2D.h>
#include "game/entityxlayer.h"

namespace prev {

	static Application * s_Instance = nullptr;

	static void testfunc() {
		Tiles tile(68, 35);
		const Shader * shader = ShaderManager::LoadShader("TempShader", "C:/users/preve/desktop/shader.vert", "C:/users/preve/desktop/shader.frag");
		shader->LoadUniform(0, shader->GetUniformLocation("TextureCoord"));
		const Texture * texture = TextureManager::LoadTexture("TempTexture", "C:/users/preve/desktop/hotel-sign.png");
		const Texture * texture2 = TextureManager::LoadTexture("TempTexture2", "C:/users/preve/desktop/banners.png");
		auto layer = new EntityXLayer();
		auto entity = layer->entities.create();
		auto entity2 = layer->entities.create();
		entity.assign<components::Position>(tile.GetTilePosition(1, 1));
		entity.assign<components::Scale>(tile.GetTileSize());
		entity.assign<components::Renderable>(shader);
		entity.assign<components::TextureComp>(texture);

		entity2.assign<components::Position >(tile.GetTilePosition(2, 2));
		entity2.assign<components::Renderable>(shader);
		entity2.assign<components::TextureComp>(texture2);

		layer->AddSystem<systems::RenderSystem>();
		s_Instance->PushLayer(layer);

		/*----------------------------------------------------- Box2D */
		b2Vec2 gravity(0.0f, -10.0f);

		// Construct a world object, which will hold and simulate the rigid bodies.
		b2World world(gravity);

		// Define the ground body.
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0.0f, -10.0f);

		// Call the body factory which allocates memory for the ground body
		// from a pool and creates the ground box shape (also from a pool).
		// The body is also added to the world.
		b2Body * groundBody = world.CreateBody(&groundBodyDef);

		// Define the ground box shape.
		b2PolygonShape groundBox;

		// The extents are the half-widths of the box.
		groundBox.SetAsBox(50.0f, 10.0f);

		// Add the ground fixture to the ground body.
		groundBody->CreateFixture(&groundBox, 0.0f);

		// Define the dynamic body. We set its position and call the body factory.
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0.0f, 4.0f);
		b2Body * body = world.CreateBody(&bodyDef);

		// Define another box shape for our dynamic body.
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(1.0f, 1.0f);

		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;

		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = 1.0f;

		// Override the default friction.
		fixtureDef.friction = 0.3f;

		// Add the shape to the body.
		body->CreateFixture(&fixtureDef);

		// Prepare for simulation. Typically we use a time step of 1/60 of a
		// second (60Hz) and 10 iterations. This provides a high quality simulation
		// in most game scenarios.
		float32 timeStep = 1.0f / 60.0f;
		int32 velocityIterations = 6;
		int32 positionIterations = 2;

		// This is our little game loop.
		for (int32 i = 0; i < 60; ++i) {
			// Instruct the world to perform a single step of simulation.
			// It is generally best to keep the time step and iterations fixed.
			world.Step(timeStep, velocityIterations, positionIterations);

			// Now print the position and angle of the body.
			b2Vec2 position = body->GetPosition();
			float32 angle = body->GetAngle();

			printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
		}

		/*----------------------------------------------------- ~Box2D */

	}

	Application::Application() {
		s_Instance = this; //Do this first
		PV_CORE_INFO("Engine Starting UP!");
		m_Window = std::shared_ptr<Window>(Window::Create());				  // Create Window based on platform
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));	 // Set EventCallback
		m_Input = std::shared_ptr<Input>(new Input(m_Window));				// Create Input Class
		Math::Init(0.01f);
#ifdef PV_RENDERING_API_OPENGL
		#pragma comment(lib, "opengl32.lib")
		m_Window->CreateOpenGLContext();
#else
		#error Define PV_RENDERING_API_OPENGL as DirectX is currently not supported
#endif
		// Create Graphics Class based on api
		PushLayer(API::Create());
		PushOverlay(new ImGuiLayer());
		testfunc();
	}

	Application::~Application() {
	}

	void Application::OnEvent(Event &event) {
		Math::OnEvent(event);
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		m_LayerStack.OnEvent(event);
		m_Input->OnEvent(event);
	}

	void Application::PushLayer(Layer * layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer * layer) {
		m_LayerStack.PushOverlay(layer);
	}

	Application * Application::GetApplicationInstance() {
		return s_Instance;
	}

	void Application::Run() {
		while (m_Running) {
			Timer::Update();

			m_LayerStack.OnUpdate();

			m_Input->OnUpdate();
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent &e) {
		m_Running = false;
		return true;
	}

}