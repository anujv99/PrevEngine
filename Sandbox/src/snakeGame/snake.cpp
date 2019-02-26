#include "pch.h"
#include "snake.h"

using namespace prev;

#include "snakenode.h"

#define WALL_THICKNESS			20
#define SPEED					9.0f * Timer::GetDeltaTime()
#define INITIAL_SNAKE_LENGTH	50
#define START_SNAKE_POS_X		5
#define START_SNAKE_POS_Y		5
#define TILE_SIZE				16
#define UPDATE_INTERVAL			0.05f // In Seconds

Level		* s_Level			= nullptr;
LevelLayer	* s_LevelLayer		= nullptr;
SnakeNode	* s_HeadNode		= nullptr;
SnakeNode	* s_TailNode		= nullptr;

unsigned int s_WindowWidth		= 0;
unsigned int s_WindowHeight		= 0;

glm::vec2 s_MaxTiles			= {-1, -1};

Tiles s_Tile(0, 0);

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
		if (m_PassedTime >= UPDATE_INTERVAL) {
			m_PassedTime = 0.0f;
			Move();
		}
		if (Input::IsKeyPressed(PV_KEYBOARD_KEY_A)) {
			m_Direction = Snake_Direction::LEFT;
			PV_INFO("A Pressed");
		}
		if (Input::IsKeyPressed(PV_KEYBOARD_KEY_D)) {
			m_Direction = Snake_Direction::RIGHT;
			PV_INFO("D Pressed");
		}
		if (Input::IsKeyPressed(PV_KEYBOARD_KEY_W)) {
			m_Direction = Snake_Direction::UP;
			PV_INFO("W Pressed");
		}
		if (Input::IsKeyPressed(PV_KEYBOARD_KEY_S)) {
			m_Direction = Snake_Direction::DOWN;
			PV_INFO("S Pressed");
		}
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

	s_Tile = Tiles(TILE_SIZE, TILE_SIZE);
	s_MaxTiles = { s_WindowWidth / TILE_SIZE, s_WindowHeight / TILE_SIZE };
	for (unsigned int i = 0; i < INITIAL_SNAKE_LENGTH; i++) {
		glm::vec2 tilePos;
		if (s_HeadNode == nullptr) {
			tilePos = s_Tile.GetTilePosition(START_SNAKE_POS_X, START_SNAKE_POS_Y);
			s_HeadNode = new SnakeNode();
			s_TailNode = s_HeadNode;
			s_TailNode->m_TilePos = { START_SNAKE_POS_X, START_SNAKE_POS_Y };
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

void SnakeGame::CreateSnake(prev::Application * app) {
	if (app == nullptr) {
		PV_ERROR("Create new application first");
		return;
	}

	const Shader * shader = ShaderManager::LoadShader("TempShader", "C:/users/preve/desktop/shader.vert", "C:/users/preve/desktop/shader.frag");
	
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
}

void SnakeGame::ReleaseSnake() {
	auto node = s_TailNode;
	while (node->m_NextNode != nullptr) {
		auto tempNode = node->m_NextNode;
		delete node;
		node = tempNode;
	}
	delete node;
}
