#include "pch.h"
#include <prev.h>

#include "lua/luascript.h"

using namespace prev;

static int tileDataLocaion = -1;

struct AnimationComponent {
	AnimationComponent(std::string spriteName, std::string path, std::string extension, unsigned int numFrames, float frameDelta) :
		animatedSprite(spriteName, path, extension, numFrames, frameDelta) {

	}
	AnimatedSprite animatedSprite;
};

struct RenderSystem : entityx::System<RenderSystem> {
	virtual void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override {
		entities.each<components::Renderable>([](entityx::Entity entity, components::Renderable & renderable) -> void {
			glm::vec2 pos = entity.component<components::Position>()->position;
			glm::vec2 scale = entity.component<components::Scale>()->scale;
			const Shader * shader = ShaderManager::GetActiveShader();
			float rotation = 0.0f;
			if (entity.has_component<components::Rotation>()) {
				rotation = entity.component<components::Rotation>()->angle;
			}
			if (entity.has_component<components::TextureComp>()) {
				entity.component<components::TextureComp>()->texture->UseTexture(0);
				shader->LoadUniform(glm::vec4(0), tileDataLocaion);
			} else if (entity.has_component<components::TiledTextureComp>()) {
				auto textureHandle = entity.component<components::TiledTextureComp>();
				textureHandle->texture->GetTexture()->UseTexture(0);
				shader->LoadUniform(glm::vec4(1, textureHandle->tileToUse,
											  textureHandle->texture->GetTileSizeX(),
											  textureHandle->texture->GetTileSizeY()),
									tileDataLocaion);
			} else if (entity.has_component<AnimationComponent>()) {
				shader->LoadUniform(glm::vec4(0), tileDataLocaion);
				entity.component<AnimationComponent>()->animatedSprite.Update();
				entity.component<AnimationComponent>()->animatedSprite.GetActiveTexture()->UseTexture(0);
			}
			BaseRenderer::RenderQuad(shader, pos, scale, rotation);
		});
	}
};

// All path are relative
WindowProps ReadConfigFile(std::string & fileName) {
	LuaScript config(fileName);
	WindowProps props;
	props.Title = config.get<std::string>("Window.Title");
	props.Width = config.get<int>("Window.Width");
	props.Height = config.get<int>("Window.Height");
	config.clean();
	return props;
}

void ReadTempLevel(std::string & rootPath, std::string & fileName) {
	LuaScript script(rootPath + "\\" + fileName);

	const Shader * shader = ShaderManager::LoadShader("TempShader", "C:/users/preve/desktop/shader.vert", "C:/users/preve/desktop/shader.frag");
	auto level = LevelManager::CreateEmptyLevel("firstLevel");
	LevelManager::StartLevel(level);
	auto levelLayer = level->CreateNewLevelLayer(true);
	levelLayer->SetShader(shader);

	tileDataLocaion = shader->GetUniformLocation("tileData");
	auto texLocation = shader->GetUniformLocation("firstTexture");
	shader->LoadUniform(0, texLocation);

	const TiledTexture * texture = TextureManager::LoadTiledTexture("TempTexture", "C:/users/preve/desktop/tileset.png", 24, 16);

	int layerWidth = script.get<int>("Scene.layers.A.width");
	int layerHeight = script.get<int>("Scene.layers.A.height");

	std::vector<int> tileDataA = script.getIntVector("Scene.layers.A.chunks.A.data");
	std::vector<int> tileDataB = script.getIntVector("Scene.layers.A.chunks.B.data");

	Tiles tile(40, 45);

	auto layer = levelLayer->GetEntityXLayer();

	int index = 0;
	for (int i = 15; i >=0; i--) {
		for (int j = 0; j < 16; j++) {

			if (tileDataA[index] == 0) {
				index++;
				continue;
			}

			auto entity = layer->entities.create();
			entity.assign<components::Position>(tile.GetTilePosition(j, i));
			entity.assign<components::Scale>(tile.GetTileSize());
			entity.assign<components::Renderable>();
			entity.assign<components::Collision>(b2_staticBody, 0.3, 1.0);
			int tile = tileDataA[index] - 1;
			entity.assign<components::TiledTextureComp>(texture, 24 * 15 - tile + 2 * (tile % 24));
			index++;
		}
	}

	index = 0;
	for (int i = 15; i >= 0; i--) {
		for (int j = 16; j < 32; j++) {

			if (tileDataB[index] == 0) {
				index++;
				continue;
			}

			auto entity = layer->entities.create();
			entity.assign<components::Position>(tile.GetTilePosition(j, i));
			entity.assign<components::Scale>(tile.GetTileSize());
			entity.assign<components::Renderable>();
			entity.assign<components::Collision>(b2_staticBody, 0.3, 1.0);
			int tile = tileDataB[index] - 1;
			entity.assign<components::TiledTextureComp>(texture, 24 * 15 - tile + 2 * (tile % 24));
			index++;

		}
	}

	levelLayer->Configure();
	layer->AddSystem<RenderSystem>();
	layer->AddSystem<systems::CollisionSystem>();

	levelLayer = level->CreateNewLevelLayer(levelLayer->GetBox2DWorld());
	levelLayer->SetShader(shader);

	layer = levelLayer->GetEntityXLayer();

	auto entity = layer->entities.create();
	entity.assign<components::Position>(tile.GetTilePosition(15, 10));
	entity.assign<components::Scale>(tile.GetTileSize() * 2.0f);
	entity.assign<AnimationComponent>("tempAnimSprite", rootPath + "\\animSprite\\", ".png", 8, 0.09f);
	entity.assign<components::Collision>(b2_dynamicBody, 0.3, 1.0);
	entity.assign<components::Renderable>();

	levelLayer->Configure();
	layer->AddSystem<RenderSystem>();
	layer->AddSystem<systems::CollisionSystem>();
}

class TestLayer : public prev::Layer {
public:
	virtual void OnUpdate() override {
		if (prev::Input::IsKeyDown(PV_KEYBOARD_KEY_A)) {
			PV_TRACE("A Pressed");
		}
	}
};

int main() {
	//Log::ShouldShowLogOnTerminal(false);
	
	auto path = Window::GetExePath();
	auto app = new Application(ReadConfigFile(path + "\\config.lua"));
	app->PushLayer(new TestLayer());
	app->GetWindow().SetVSync(true);

	ReadTempLevel(path, std::string("untitled.lua"));
	//Add things here
	/*--------------------------------------------*/
	/*auto level = LevelManager::CreateEmptyLevel("firstLevel");
	LevelManager::StartLevel(level);

	Tiles tile(40, 45);
	const TiledTexture * texture = TextureManager::LoadTiledTexture("TempTexture", "C:/users/preve/desktop/tileset.png", 24, 16);
	const Texture * texture2 = TextureManager::LoadTexture("TempTexture2", "C:/users/preve/desktop/banners.png");
	const Shader * shader = ShaderManager::LoadShader("TempShader", "C:/users/preve/desktop/shader.vert", "C:/users/preve/desktop/shader.frag");
	tileDataLocaion = shader->GetUniformLocation("tileData");
	level->SetShader(shader);

	auto texLocation = shader->GetUniformLocation("firstTexture");
	shader->LoadUniform(0, texLocation);
	auto layer = level->GetEntityXLayer();
	auto entity = layer->entities.create();
	auto entity2 = layer->entities.create();
	entity.assign<components::Position>(tile.GetTilePosition(5, 0));
	entity.assign<components::Scale>(tile.GetTileSize());
	entity.assign<components::Renderable>();
	entity.assign<components::TiledTextureComp>(texture, 101);
	entity.assign<components::Collision>(b2BodyType::b2_staticBody, 0.3f, 1.0f);

	entity2.assign<components::Position >(tile.GetTilePosition(5, 10));
	entity2.assign<components::Scale>(tile.GetTileSize());
	entity2.assign<components::Rotation>(43.0f);
	entity2.assign<components::Renderable>();
	entity2.assign<components::TextureComp>(texture2);
	entity2.assign<components::Collision>(b2BodyType::b2_dynamicBody, 0.3f, 1.0f);

	level->Configure();
	layer->AddSystem<RenderSystem>();
	layer->AddSystem<systems::CollisionSystem>();*/
	/*--------------------------------------------*/
	app->Run();
	delete app;
	return 0;
}