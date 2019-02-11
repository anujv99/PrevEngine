#include "pch.h"
#include <prev.h>

using namespace prev;

class TestLayer : public Layer {
public:
	TestLayer() { }
	~TestLayer() { }
	void OnAttach() override {
	}
	void OnDetach() override {

	}
	void OnUpdate() override {
		PV_INFO("%f", Timer::GetDeltaTime());
	}
	void OnEvent(Event &e) override {

	}
};


int main() {
	//Log::ShouldShowLogOnTerminal(false);
	auto app = new Application();
	app->GetWindow().SetVSync(true);
	//Add things here
	/*--------------------------------------------*/
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

	entity2.assign<components::Position > (tile.GetTilePosition(2, 2));
	entity2.assign<components::Renderable>(shader);
	entity2.assign<components::TextureComp>(texture2);

	layer->AddSystem<systems::RenderSystem>();
	app->PushLayer(layer);

	/*--------------------------------------------*/
	app->Run();
	delete app;
	return 0;
}