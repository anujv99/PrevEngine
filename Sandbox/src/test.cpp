#include "pch.h"
#include <prev.h>

#include "lua/luascript.h"
#include "snakeGame/snake.h"

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

static WindowProps ReadWindowProperties(std::string configFile) {
	LuaScript config(configFile);
	Log::ShouldShowLogOnTerminal(config.get<bool>("Log.IsLogEnabled"));
	Timer::FPSCounter(config.get<bool>("Log.ShowFPS"));

	WindowProps props;
	props.CType = CursorType::PV_HAND;
	props.Title = config.get<std::string>("Window.Title");
	props.Width = config.get<int>("Window.Width");
	props.Height = config.get<int>("Window.Height");
	return props;
}

static void SetVsync(std::string configFile, Application * app) {
	LuaScript config(configFile);
	app->GetWindow().SetVSync(config.get<bool>("Window.IsVsync"));
}

int main() {
	auto path = Window::GetExePath();
	auto app = new Application(ReadWindowProperties(path + "../../../Sandbox/res/config.lua"));
	SetVsync(path + "config.lua", app);

	SnakeGame::CreateSnake(app, path + "../../../Sandbox/res/snakeConfig.lua");

	//Add things here
	/*--------------------------------------------*/

	/*--------------------------------------------*/
	app->Run();

	SnakeGame::ReleaseSnake();

	delete app;
	return 0;
}