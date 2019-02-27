#include "pch.h"
#include "snake.h"

using namespace prev;

#include "snakenode.h"

#define WALL_THICKNESS			20

static float		s_UpdateInterval		= 0.0f;
static unsigned int s_InitialSnakeLength	= 0;
static unsigned int s_SnakeStartX			= 0;
static unsigned int s_SnakeStartY			= 0;
static unsigned int s_TileSize				= 0;
static unsigned int s_WindowWidth			= 0;
static unsigned int s_WindowHeight			= 0;
static unsigned int s_LengthToIncrease		= 1;
static unsigned int s_SnakeLength			= 0;

static Level		* s_Level				= nullptr;
static LevelLayer	* s_LevelLayer			= nullptr;
static SnakeNode	* s_HeadNode			= nullptr;
static SnakeNode	* s_TailNode			= nullptr;

static glm::vec2	s_MaxTiles				= {-1, -1};
static glm::vec2	s_FoodIndex				= { -1, -1 };

static Tiles		s_Tile(0, 0);


static void ReadConfigFile(std::string filePath) {
	LuaScript snakeConfig(filePath);
	
	s_LengthToIncrease		= snakeConfig.get<int>("Snake_Config.LengthToIncrease");
	s_UpdateInterval		= snakeConfig.get<float>("Snake_Config.UpdateInterval");
	s_InitialSnakeLength	= snakeConfig.get<int>("Snake_Config.InitialLength");
	s_SnakeStartX			= snakeConfig.get<int>("Snake_Config.StartPos.X");
	s_SnakeStartY			= snakeConfig.get<int>("Snake_Config.StartPos.Y");
	s_TileSize				= snakeConfig.get<int>("Snake_Config.TileSize");
}

class FoodLayer : public Layer {
public:
	FoodLayer() {
		auto foodEntity = s_LevelLayer->GetEntityXLayer()->entities.create();
		foodEntity.assign<components::Position>(glm::vec2(-1, -1));
		foodEntity.assign<components::Scale>(s_Tile.GetTileSize());
		foodEntity.assign<components::Renderable>();
		m_ActiveFood.m_Entity = foodEntity;
		m_ActiveFood.m_TilePos = { -1, -1 };
		CreateFood();
	}
	virtual void OnUpdate() override {
		if (s_HeadNode->m_TilePos == m_ActiveFood.m_TilePos) {
			PV_INFO("On Food");
			IncreaseSnakeLength();
			CreateFood();
		}
	}
private:
	void CreateFood() {
		glm::vec2 tileIndex;

		srand(Timer::GetTime() * 2721002245744);
		tileIndex.x = rand() % int(s_MaxTiles.x);
		srand(Timer::GetTime() * 9464794649828);
		tileIndex.y = rand() % int(s_MaxTiles.y);

		m_ActiveFood.m_Entity.component<components::Position>()->position = s_Tile.GetTilePosition(tileIndex.x, tileIndex.y);
		m_ActiveFood.m_TilePos = tileIndex;
	}
	void IncreaseSnakeLength() {
		SnakeNode * node = new SnakeNode();
		node->m_NextNode = s_TailNode;
		s_TailNode->m_TilePos = s_TailNode->m_TilePos + glm::vec2(-1, 0);
		s_TailNode = node;

		auto entity = s_LevelLayer->GetEntityXLayer()->entities.create();
		entity.assign<components::Position>(s_Tile.GetTilePosition(s_TailNode->m_TilePos.x, s_TailNode->m_TilePos.y));
		entity.assign<components::Scale>(s_Tile.GetTileSize());
		entity.assign<components::Renderable>();
		s_TailNode->m_Entity = entity;
		s_SnakeLength++;
	}
private:
	FoodNode m_ActiveFood;
};

class MovementLayer : public Layer {
private:
	enum class Snake_Direction {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};
public:
	virtual void OnUpdate() override {
		m_PassedTime += Timer::GetDeltaTime();
		if (m_PassedTime >= s_UpdateInterval) {
			m_PassedTime = 0.0f;
			Move();
		}
		if (Input::IsKeyPressed(PV_KEYBOARD_KEY_A) && m_Direction != Snake_Direction::RIGHT) {
			m_Direction = Snake_Direction::LEFT;
			PV_INFO("Direction LEFT");
		}
		if (Input::IsKeyPressed(PV_KEYBOARD_KEY_D) && m_Direction != Snake_Direction::LEFT) {
			m_Direction = Snake_Direction::RIGHT;
			PV_INFO("Direction RIGHT");
		}
		if (Input::IsKeyPressed(PV_KEYBOARD_KEY_W) && m_Direction != Snake_Direction::DOWN) {
			m_Direction = Snake_Direction::UP;
			PV_INFO("Direction UP");
		}
		if (Input::IsKeyPressed(PV_KEYBOARD_KEY_S) && m_Direction != Snake_Direction::UP) {
			m_Direction = Snake_Direction::DOWN;
			PV_INFO("Direction DOWN");
		}
		if (s_SnakeLength > 3)
			CheckForCollision();
	}
private:
	void CheckForCollision() {
		auto headPos = s_HeadNode->m_TilePos;
		auto node = s_TailNode;
		while (node->m_NextNode != nullptr) {
			if (headPos == node->m_TilePos) {
				PV_ERROR("GAME OVER");
			}
			node = node->m_NextNode;
		}
	}
	void Move() {
		glm::vec2 indexToAdd;

		switch (m_Direction) {
		case MovementLayer::Snake_Direction::UP:
			indexToAdd = { 0, 1 };
			break;
		case MovementLayer::Snake_Direction::DOWN:
			indexToAdd = { 0, -1 };
			break;
		case MovementLayer::Snake_Direction::LEFT:
			indexToAdd = { -1, 0 };
			break;
		case MovementLayer::Snake_Direction::RIGHT:
			indexToAdd = { 1, 0 };
			break;
		default:
			break;
		}

		auto node = s_TailNode;
		s_TailNode = node->m_NextNode;
		glm::vec2 tileIndex = s_HeadNode->m_TilePos;
		tileIndex += indexToAdd;

		if (tileIndex.x >= s_MaxTiles.x) {
			tileIndex.x = 0;
		} else if (tileIndex.x < 0) {
			tileIndex.x = s_MaxTiles.x - 1;
		}

		if (tileIndex.y >= s_MaxTiles.y) {
			tileIndex.y = 0;
		} else if (tileIndex.y < 0) {
			tileIndex.y = s_MaxTiles.y - 1;
		}

		if (s_SnakeLength == 1) {
			s_HeadNode->m_Entity.component<components::Position>()->position = s_Tile.GetTilePosition(tileIndex.x, tileIndex.y);
			s_HeadNode->m_TilePos = tileIndex;
			return;
		}

		s_HeadNode->m_NextNode = node;
		s_HeadNode = node;
		s_HeadNode->m_Entity.component<components::Position>()->position = s_Tile.GetTilePosition(tileIndex.x, tileIndex.y);
		s_HeadNode->m_TilePos = tileIndex;
		s_HeadNode->m_NextNode = nullptr;
	}
private:
	Snake_Direction m_Direction = Snake_Direction::LEFT;
	float m_PassedTime = 0;
};

void CreateSnakeEntity(LevelLayer * levelLayer) {

	s_Tile = Tiles(s_TileSize, s_TileSize);
	s_MaxTiles = { s_WindowWidth / s_TileSize, s_WindowHeight / s_TileSize };
	s_SnakeStartX = s_SnakeStartX % int(s_MaxTiles.x);
	s_SnakeStartY = s_SnakeStartY % int(s_MaxTiles.y);
	for (unsigned int i = 0; i < s_InitialSnakeLength; i++) {
		glm::vec2 tilePos;
		if (s_HeadNode == nullptr) {
			tilePos = s_Tile.GetTilePosition(s_SnakeStartX, s_SnakeStartY);
			s_HeadNode = new SnakeNode();
			s_TailNode = s_HeadNode;
			s_TailNode->m_TilePos = { s_SnakeStartX, s_SnakeStartY };
			s_TailNode->m_NextNode = s_TailNode;
		} else {
			tilePos = s_Tile.GetTilePosition(s_TailNode->m_TilePos.x + 1, s_TailNode->m_TilePos.y);
			auto snakeNode = new SnakeNode();
			snakeNode->m_NextNode = s_TailNode;
			snakeNode->m_TilePos = { s_TailNode->m_TilePos.x + 1, s_TailNode->m_TilePos.y };
			s_TailNode = snakeNode;
		}

		auto entity = levelLayer->GetEntityXLayer()->entities.create();
		entity.assign<components::Position>(tilePos);
		entity.assign<components::Scale>(s_Tile.GetTileSize());
		entity.assign<components::Renderable>();
		s_TailNode->m_Entity = entity;
		s_SnakeLength++;
	}

}

void CreateWalls(LevelLayer * levelLayer) {

	//For vertical walls
	{
		//Left
		Tiles tile(WALL_THICKNESS, s_WindowHeight);
		auto entity = levelLayer->GetEntityXLayer()->entities.create();
		entity.assign<components::Position>(tile.GetTilePosition(0, 0));
		entity.assign<components::Scale>(tile.GetTileSize());
		entity.assign<components::Collision>(b2_staticBody, 0.0, 1.0);

		//Right
		entity = levelLayer->GetEntityXLayer()->entities.create();
		entity.assign<components::Position>(tile.GetTilePosition(int(s_WindowWidth / WALL_THICKNESS) - 1, 0));
		entity.assign<components::Scale>(tile.GetTileSize());
		entity.assign<components::Collision>(b2_staticBody, 0.0, 1.0);
	}

	//For horizontal walls
	{
		//Bottom
		Tiles tile(s_WindowWidth, WALL_THICKNESS);
		auto entity = levelLayer->GetEntityXLayer()->entities.create();
		entity.assign<components::Position>(tile.GetTilePosition(0, 0));
		entity.assign<components::Scale>(tile.GetTileSize());
		entity.assign<components::Collision>(b2_staticBody, 0.0, 1.0);

		//Top
		entity = levelLayer->GetEntityXLayer()->entities.create();
		entity.assign<components::Position>(tile.GetTilePosition(0, int(s_WindowHeight / WALL_THICKNESS) - 1));
		entity.assign<components::Scale>(tile.GetTileSize());
		entity.assign<components::Collision>(b2_staticBody, 0.0, 1.0);
	}

}

void SnakeGame::CreateSnake(prev::Application * app, std::string configFilePath) {
	if (app == nullptr) {
		PV_ERROR("Create new application first");
		return;
	}

	ReadConfigFile(configFilePath);

	const Shader * shader = ShaderManager::LoadShader("SnakeShader", 
													  (Window::GetExePath() + "Shaders\\shader.vert").c_str(),
													  (Window::GetExePath() + "Shaders\\shader.frag").c_str());
	
	s_WindowWidth = app->GetApplicationInstance()->GetWindow().GetWidth();
	s_WindowHeight = app->GetApplicationInstance()->GetWindow().GetHeight();

	s_Level = LevelManager::CreateEmptyLevel("SnakeLevel");
	LevelManager::StartLevel(s_Level);
	s_LevelLayer = s_Level->CreateNewLevelLayer(true, {0, 0});
	s_LevelLayer->SetShader(shader);

	CreateSnakeEntity(s_LevelLayer);
	//CreateWalls(s_LevelLayer);

	s_LevelLayer->GetEntityXLayer()->AddSystem<systems::RenderSystem>();
	s_LevelLayer->GetEntityXLayer()->AddSystem<systems::CollisionSystem>();

	s_LevelLayer->Configure();

	app->PushLayer(new MovementLayer());
	app->PushLayer(new FoodLayer());
}

void SnakeGame::ReleaseSnake() {
	if (s_SnakeLength == 1) {
		delete s_TailNode;
		s_TailNode = nullptr;
		return;
	}
	auto node = s_TailNode;
	while (node->m_NextNode != nullptr) {
		auto tempNode = node->m_NextNode;
		delete node;
		node = tempNode;
	}
	delete node;
}
