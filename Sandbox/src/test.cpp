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

class TestLayer : public Layer {
public:
	TestLayer() {
		m_Buffer = InstancedBuffer::Create(1024);
		m_Shader = ShaderManager::LoadShader("InstancedShader",
											 (Window::GetExePath() + "../../../Sandbox/res/Shaders/instshader.vert").c_str(),
											 (Window::GetExePath() + "../../../Sandbox/res/Shaders/instshader.frag").c_str());

		float gg[] = {
			0, 0,
			1, 1,
			2, 2,
			3, 3,
			4, 4
		};

		m_Buffer->AppendData(sizeof(gg), gg);
		m_Buffer->SetNumerOfInstances(5);

		Tiles tile(16, 16);
		m_ModelMatrix = glm::mat4(1.0f);
		m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(tile.GetTilePosition(10, 10), 0));
		m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(tile.GetTileSize(), 0.0f));
		m_Shader->LoadProjectionMatrix(Math::GetProjectionMatrix());
		m_Shader->LoadModelMatrix(m_ModelMatrix);
	}
	virtual void OnUpdate() override {
		m_Shader->UseShader();
		BaseRenderer::RenderQuadInstanced(m_Buffer);
		if (Input::IsKeyPressed(PV_KEYBOARD_KEY_P)) {
			float gg[] = {
				5, 5,
				6, 6,
				7, 7,
				8, 8,
				9, 9
			};

			m_Buffer->ReplaceData(0, sizeof(gg), gg);
			m_Buffer->SetNumerOfInstances(5);
		}
	}
	~TestLayer() {
		delete m_Buffer;
	}
private:
	InstancedBuffer * m_Buffer;
	const Shader * m_Shader;
	glm::mat4 m_ModelMatrix;
};

#define CONFIG_FILE "../../../Sandbox/res/snakeConfig.lua"

int main() {
	auto path = Window::GetExePath();
	auto app = new Application(ReadWindowProperties(path + CONFIG_FILE));
	SetVsync(path + CONFIG_FILE, app);

	SnakeGame::CreateSnake(app, path + CONFIG_FILE);

	//Add things here
	/*--------------------------------------------*/
	//app->PushLayer(new TestLayer());
	/*--------------------------------------------*/
	app->Run();

	SnakeGame::ReleaseSnake();

	delete app;
	return 0;
}