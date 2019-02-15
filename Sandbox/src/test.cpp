#include "pch.h"
#include <prev.h>

#include "lua/luascript.h"

using namespace prev;

// All path are relative
WindowProps ReadConfigFile(std::string & fileName) {
	LuaScript config(fileName);
	WindowProps props;
	props.Title = config.get<std::string>("Window.Title");
	props.Width = config.get<int>("Window.Width");
	props.Height = config.get<int>("Window.Height");
	return props;
}

int main() {
	//Log::ShouldShowLogOnTerminal(false);
	
	auto path = Window::GetExePath();
	auto app = new Application(ReadConfigFile(path + "/config.lua"));
	app->GetWindow().SetVSync(true);
	//Add things here
	/*--------------------------------------------*/
	auto level = LevelManager::CreateEmptyLevel("firstLevel");
	LevelManager::StartLevel(level);

	Tiles tile(64, 64);
	const Texture * texture = TextureManager::LoadTexture("TempTexture", "C:/users/preve/desktop/hotel-sign.png");
	const Texture * texture2 = TextureManager::LoadTexture("TempTexture2", "C:/users/preve/desktop/banners.png");
	const Shader * shader = ShaderManager::LoadShader("TempShader", "C:/users/preve/desktop/shader.vert", "C:/users/preve/desktop/shader.frag");
	level->SetShader(shader);

	auto texLocation = shader->GetUniformLocation("firstTexture");
	shader->LoadUniform(0, texLocation);
	auto layer = level->GetEntityXLayer();
	auto entity = layer->entities.create();
	auto entity2 = layer->entities.create();
	entity.assign<components::Position>(tile.GetTilePosition(5, 0));
	entity.assign<components::Scale>(tile.GetTileSize());
	entity.assign<components::Renderable>();
	entity.assign<components::TextureComp>(texture);
	entity.assign<components::Collision>(b2BodyType::b2_staticBody, 0.3f, 1.0f);

	entity2.assign<components::Position >(tile.GetTilePosition(5, 10));
	entity2.assign<components::Scale>(tile.GetTileSize());
	entity2.assign<components::Rotation>(43.0f);
	entity2.assign<components::Renderable>();
	entity2.assign<components::TextureComp>(texture2);
	entity2.assign<components::Collision>(b2BodyType::b2_dynamicBody, 0.3f, 1.0f);

	level->Configure();
	layer->AddSystem<systems::RenderSystem>();
	layer->AddSystem<systems::CollisionSystem>();
	/*--------------------------------------------*/
	app->Run();
	delete app;
	return 0;
}