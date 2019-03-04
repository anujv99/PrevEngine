#include "pch.h"
#include "snake.h"

using namespace prev;

#include "snakenode.h"

#define WALL_THICKNESS			20
#define MAX_SNAKE_LENGTH		10000

static float		s_UpdateInterval		= 0.0f;
static unsigned int s_InitialSnakeLength	= 0;
static unsigned int s_SnakeStartX			= 0;
static unsigned int s_SnakeStartY			= 0;
static unsigned int s_TileSize				= 0;
static unsigned int s_WindowWidth			= 0;
static unsigned int s_WindowHeight			= 0;
static unsigned int s_LengthToIncrease		= 1;
static unsigned int s_SnakeLength			= 0;

static int s_IndexToChangeInBuffer			= 0;

static Level		* s_Level				= nullptr;
static LevelLayer	* s_LevelLayer			= nullptr;
static LevelLayer	* s_FoodLayer			= nullptr;
static SnakeNode	* s_HeadNode			= nullptr;
static SnakeNode	* s_TailNode			= nullptr;

static InstancedBuffer * s_InstancedBuffer	= nullptr;

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

class RenderLayer : public Layer {
public:
	RenderLayer() {
		m_GlobalModelMatrix = glm::mat4(1.0f);
		m_GlobalModelMatrix = glm::translate(m_GlobalModelMatrix, glm::vec3(s_Tile.GetTilePosition(0, 0), 0.0f));
		m_GlobalModelMatrix = glm::scale(m_GlobalModelMatrix, glm::vec3(s_Tile.GetTileSize(), 0.0f));
		m_Shader = s_LevelLayer->GetShader();
		m_Shader->UseShader();
		m_Shader->LoadProjectionMatrix(Math::GetProjectionMatrix());
		m_Shader->LoadModelMatrix(m_GlobalModelMatrix);
	}
	virtual void OnUpdate() override {
		m_Shader->UseShader();
		BaseRenderer::RenderQuadInstanced(s_InstancedBuffer);
	}
private:
	const Shader * m_Shader;
	glm::mat4 m_GlobalModelMatrix;
};

class FoodLayer : public Layer {
public:
	FoodLayer() {
		auto foodEntity = s_FoodLayer->GetEntityXLayer()->entities.create();
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
		s_TailNode = node;
		//s_TailNode->m_TilePos = s_TailNode->m_TilePos + glm::vec2(-1, 0);
		s_TailNode->m_TilePos = s_TailNode->m_TilePos + (s_TailNode->m_TilePos - s_TailNode->m_NextNode->m_TilePos);
		s_SnakeLength++;
		/*
		 *	Can be optimized in several ways:
		 *	One way: If the length of the snake is large enough, then create 
		 *			 a buffer to hold the nodes if the snake eats food. Then
		 *			 when the snake is at the starting position i.e. when the
		 *			 s_IndexToChangeInBuffer is 0 (might be different) then add
		 *			 the node to the list.
		 */
		//if (s_SnakeLength < 20)
			RecreateSnake();
	}
	void RecreateSnake() {
		std::vector<float> allPos;
		allPos.resize(s_SnakeLength * 2);
		int i = 2 * s_SnakeLength - 1;
		auto node = s_TailNode;
		while (node != nullptr) {
			allPos[i--] = node->m_TilePos.y;
			allPos[i--] = node->m_TilePos.x;
			node = node->m_NextNode;
		}
		s_InstancedBuffer->ReplaceData(0, allPos.size() * sizeof(float), &allPos[0]);
		s_InstancedBuffer->SetNumerOfInstances(allPos.size() / 2);
		s_IndexToChangeInBuffer = (sizeof(float) * allPos.size()) - (2 * sizeof(float));
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

		s_HeadNode->m_NextNode = node;
		s_HeadNode = node;
		s_HeadNode->m_TilePos = tileIndex;
		s_HeadNode->m_NextNode = nullptr;

		UpdateRenderBuffer();
	}
	void UpdateRenderBuffer() {
		s_InstancedBuffer->ReplaceData(s_IndexToChangeInBuffer, 2 * sizeof(float), glm::value_ptr(s_HeadNode->m_TilePos));
		s_IndexToChangeInBuffer -= 2 * sizeof(float);
		if (s_IndexToChangeInBuffer < 0)
			s_IndexToChangeInBuffer = (s_SnakeLength * 2 * sizeof(float)) - (2 * sizeof(float));
	}
private:
	Snake_Direction m_Direction = Snake_Direction::LEFT;
	float m_PassedTime = 0;
};

void CreateSnakeEntity(LevelLayer * levelLayer) {

	std::vector<float> allTilePos;

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

		allTilePos.push_back(s_TailNode->m_TilePos.x);
		allTilePos.push_back(s_TailNode->m_TilePos.y);
		s_SnakeLength++;
	}

	s_InstancedBuffer->AppendData(sizeof(float) * allTilePos.size(), &allTilePos[0]);
	s_InstancedBuffer->SetNumerOfInstances(allTilePos.size() / 2);
	s_IndexToChangeInBuffer = (sizeof(float) * allTilePos.size()) - (2 * sizeof(float));
}

void SnakeGame::CreateSnake(prev::Application * app, std::string configFilePath) {
	if (app == nullptr) {
		PV_ERROR("Create new application first");
		return;
	}

	s_InstancedBuffer = InstancedBuffer::Create(10000 * 2 * sizeof(float));
	ReadConfigFile(configFilePath);

	const Shader * shader = ShaderManager::LoadShader("SnakeShader",
													  (Window::GetExePath() + "../../../Sandbox/res/Shaders/instshader.vert").c_str(),
													  (Window::GetExePath() + "../../../Sandbox/res/Shaders/instshader.frag").c_str());

	const Shader * foodShader = ShaderManager::LoadShader("FoodShader",
														  (Window::GetExePath() + "../../../Sandbox/res/Shaders/shader.vert").c_str(),
														  (Window::GetExePath() + "../../../Sandbox/res/Shaders/shader.frag").c_str());
	
	s_WindowWidth = app->GetApplicationInstance()->GetWindow().GetWidth();
	s_WindowHeight = app->GetApplicationInstance()->GetWindow().GetHeight();

	s_Level = LevelManager::CreateEmptyLevel("SnakeLevel");
	LevelManager::StartLevel(s_Level);
	s_LevelLayer = s_Level->CreateNewLevelLayer(true, {0, 0});
	s_LevelLayer->SetShader(shader);

	CreateSnakeEntity(s_LevelLayer);

	s_LevelLayer->Configure();

	s_FoodLayer = s_Level->CreateNewLevelLayer(false);
	s_FoodLayer->SetShader(foodShader);
	s_FoodLayer->GetEntityXLayer()->AddSystem<systems::RenderSystem>();
	s_FoodLayer->Configure();

	app->PushLayer(new MovementLayer());
	app->PushLayer(new FoodLayer());
	app->PushLayer(new RenderLayer());
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
	delete s_InstancedBuffer;
}
