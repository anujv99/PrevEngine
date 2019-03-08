#include "pch.h"
#include "tetris.h"

#include "lua/luascript.h"

#include "tetrisShapes.h"

struct TetrisNode {
	TetrisNode() :
		m_IsActive(false) {}
	bool m_IsActive;
};

static TetrisNode ** s_TetrisArray					= nullptr;

static bool s_IsShapeDown							= false;

static unsigned int s_TileDivisionX					= 0;
static unsigned int s_TileDivisionY					= 0;
static unsigned int s_TileSizeX						= 0;
static unsigned int s_TileSizeY						= 0;

static float s_UpdateInterval						= 0.0f;

static prev::Tiles s_Tile							= prev::Tiles(0, 0);

static prev::Level * s_Level						= nullptr;
static prev::LevelLayer * s_TetrisLayer				= nullptr;
static prev::InstancedBuffer * s_InstBuffer			= nullptr;

static std::vector<int> s_Top						= {};
static std::vector<int> s_Down						= {};
static std::vector<int> s_Left						= {};
static std::vector<int> s_Right						= {};
static std::vector<glm::ivec2> s_CurrentShape		= {};
static std::vector<float> s_ShapeInstBuffer			= {};

static void ReadConfigFile(const std::string & configFilePath);

class TetrisLayer : public prev::Layer {
public:
	void PrintBoolArray() {
		PV_INFO("-----------------------");
		for (unsigned int i = 0; i < s_TileDivisionY; i++) {
			std::string line;
			for (unsigned int j = 0; j < s_TileDivisionX; j++) {
				line += std::to_string(s_TetrisArray[j][i].m_IsActive) + " ";
			}
			PV_INFO("%s", line.c_str());
		}
		PV_INFO("-----------------------");
	}
	TetrisLayer() {
		m_GlobalModelMatrix = glm::mat4(1.0f);
		m_GlobalModelMatrix = glm::translate(m_GlobalModelMatrix, glm::vec3(s_Tile.GetTilePosition(0, 0), 0.0f));
		m_GlobalModelMatrix = glm::scale(m_GlobalModelMatrix, glm::vec3(s_Tile.GetTileSize(), 0.0f));
		m_Shader = s_TetrisLayer->GetShader();
		m_Shader->UseShader();
		m_Shader->LoadProjectionMatrix(prev::Math::GetProjectionMatrix());
		m_Shader->LoadModelMatrix(m_GlobalModelMatrix);
		AddShape<shapes::Square>();
	}
	virtual void OnUpdate() override {
		m_PassedTime += prev::Timer::GetDeltaTime();
		if (m_PassedTime > s_UpdateInterval) {
			MoveShapeDown();
			m_PassedTime = 0.0f;
		}

		if (prev::Input::IsKeyPressed(PV_KEYBOARD_KEY_A)) {
			MoveShapeLeft();
		}
		if (prev::Input::IsKeyPressed(PV_KEYBOARD_KEY_D)) {
			MoveShapeRight();
		}
		if (prev::Input::IsKeyPressed(PV_KEYBOARD_KEY_L)) {
			PrintBoolArray();
		}
		if (prev::Input::IsKeyPressed(PV_KEYBOARD_KEY_L)) {
			//TODO : Rotatation
		}

		s_TetrisLayer->GetShader()->UseShader();
		prev::BaseRenderer::RenderQuadInstanced(s_InstBuffer);

		if (prev::Input::IsKeyPressed(PV_KEYBOARD_KEY_O))
			AddShape<shapes::L>();
		else if (prev::Input::IsKeyPressed(PV_KEYBOARD_KEY_P))
			AddShape<shapes::Square>();
		else if (prev::Input::IsKeyPressed(PV_KEYBOARD_KEY_I))
			AddShape<shapes::LReverse>();
	}
private:
	void MoveShapeLeft() {
		bool canMove = true;
		for (auto & tile : s_Left) {
			glm::ivec2 tilePos = s_CurrentShape[tile];
			if (tilePos.x == 0) {
				canMove = false;
				break;
			}
			if (s_TetrisArray[tilePos.x - 1][tilePos.y].m_IsActive) {
				canMove = false;
				break;
			}
		}
		if (canMove) {
			int i = 0;
			std::vector<glm::ivec2> tilesToActivate;
			for (auto & tile : s_CurrentShape) {
				//s_TetrisArray[tile.x - 1][tile.y] = s_TetrisArray[tile.x][tile.y];
				s_TetrisArray[tile.x][tile.y].m_IsActive = false;
				tile.x -= 1;
				tilesToActivate.push_back(tile);

				s_InstBuffer->ReplaceData(s_ShapeInstBuffer[i], 2 * sizeof(float), glm::value_ptr(glm::vec2(tile.x, s_TileDivisionY - 1 - tile.y)));
				i++;
			}
			for (auto & tileIndex : tilesToActivate) {
				s_TetrisArray[tileIndex.x][tileIndex.y].m_IsActive = true;
			}
		}
	}
	void MoveShapeRight() {
		bool canMove = true;
		for (auto & tile : s_Right) {
			glm::ivec2 tilePos = s_CurrentShape[tile];
			if (tilePos.x == s_TileDivisionX - 1) {
				canMove = false;
				break;
			}
			if (s_TetrisArray[tilePos.x + 1][tilePos.y].m_IsActive) {
				canMove = false;
				break;
			}
		}
		if (canMove) {
			int i = 0;
			std::vector<glm::ivec2> tilesToActivate;
			for (auto & tile : s_CurrentShape) {
				//s_TetrisArray[tile.x + 1][tile.y] = s_TetrisArray[tile.x][tile.y];
				s_TetrisArray[tile.x][tile.y].m_IsActive = false;
				tile.x += 1;
				tilesToActivate.push_back(tile);

				s_InstBuffer->ReplaceData(s_ShapeInstBuffer[i], 2 * sizeof(float), glm::value_ptr(glm::vec2(tile.x, s_TileDivisionY - 1 - tile.y)));
				i++;
			}
			for (auto & tileIndex : tilesToActivate) {
				s_TetrisArray[tileIndex.x][tileIndex.y].m_IsActive = true;
			}
		}
	}
	void MoveShapeDown() {
		bool canMove = true;
		for (auto & tile : s_Down) {
			glm::ivec2 tilePos = s_CurrentShape[tile];
			if (tilePos.y == s_TileDivisionY - 1) {
				canMove = false;
				break;
			}
			if (s_TetrisArray[tilePos.x][tilePos.y + 1].m_IsActive) {
				canMove = false;
				break;
			}
		}
		if (canMove) {
			int i = 0;
			std::vector<glm::ivec2> tilesToActivate;
			for (auto & tile : s_CurrentShape) {
				//s_TetrisArray[tile.x][tile.y + 1] = s_TetrisArray[tile.x][tile.y];
				s_TetrisArray[tile.x][tile.y].m_IsActive = false;
				tile.y += 1;
				tilesToActivate.push_back(tile);

				s_InstBuffer->ReplaceData(s_ShapeInstBuffer[i], 2 * sizeof(float), glm::value_ptr(glm::vec2(tile.x, s_TileDivisionY - 1 - tile.y)));
				i++;
			}
			for (auto & tileIndex : tilesToActivate) {
				s_TetrisArray[tileIndex.x][tileIndex.y].m_IsActive = true;
			}
		}
	}
	template<typename T>
	void AddShape() {
		glm::ivec2 index = T::m_StartIndex;
		s_CurrentShape.clear();
		s_ShapeInstBuffer.clear();
		for (auto node : T::m_Shape) {
			index += node.m_Offset;

			s_TetrisArray[index.x][index.y].m_IsActive = true;

			s_CurrentShape.push_back(index);
			s_ShapeInstBuffer.push_back(s_InstBuffer->GetDataSize());

			s_InstBuffer->AppendData(2 * sizeof(float), glm::value_ptr(glm::vec2(index.x, s_TileDivisionY - 1 - index.y)));
			s_InstBuffer->SetNumerOfInstances(s_InstBuffer->GetNumberOfInstances() + 1);

		}
		CalculateExtremes();
	}
	void CalculateExtremes() {
		s_Left.clear();
		s_Right.clear();
		s_Top.clear();
		s_Down.clear();
		for (unsigned int i = 0; i < s_CurrentShape.size(); i++) {
			bool isLeft = true;
			bool isRight = true;
			bool isTop = true;
			bool isBottom = true;
			auto nodeA = s_CurrentShape[i];
			for (unsigned int j = 0; j < s_CurrentShape.size(); j++) {
				if (i == j) continue;
				auto nodeB = s_CurrentShape[j];

				if (nodeA.y == nodeB.y) {
					if (nodeA.x > nodeB.x)
						isLeft = false;
					else if (nodeA.x < nodeB.x)
						isRight = false;
				}

				if (nodeA.x == nodeB.x) {
					if (nodeA.y > nodeB.y)
						isTop = false;
					else if (nodeA.y < nodeB.y)
						isBottom = false;
				}
			}

			if (isLeft)
				s_Left.push_back(i);
			if (isRight)
				s_Right.push_back(i);
			if (isTop)
				s_Top.push_back(i);
			if (isBottom)
				s_Down.push_back(i);

		}
	}
private:
	float m_PassedTime		= 0.0f;
	const prev::Shader * m_Shader;
	glm::mat4 m_GlobalModelMatrix;
};


void Tetris::CreateTetris(prev::Application * app, const std::string & configFilePath) {
	ReadConfigFile(configFilePath);

	/*
	 * TODO // Create template of this array
	 */
	s_TetrisArray = new TetrisNode * [s_TileDivisionX];
	for (unsigned int i = 0; i < s_TileDivisionX; i++) {
		s_TetrisArray[i] = new TetrisNode[s_TileDivisionY];
	}

	s_Tile = prev::Tiles(s_TileSizeX, s_TileSizeY);
	shapes::CreateShapes();

	s_Level = prev::LevelManager::CreateEmptyLevel("tetrisLevel");
	prev::LevelManager::StartLevel(s_Level);
	s_TetrisLayer = s_Level->CreateNewLevelLayer(false);
	
	const prev::Shader * shader = prev::ShaderManager::LoadShader("FoodShader",
		(prev::Window::GetExePath() + "../../../Sandbox/res/Shaders/instshader.vert").c_str(),
		(prev::Window::GetExePath() + "../../../Sandbox/res/Shaders/instshader.frag").c_str());

	s_TetrisLayer->SetShader(shader);

	s_InstBuffer =  prev::InstancedBuffer::Create(1000 * 2 * sizeof(float));

	app->PushLayer(new TetrisLayer());
}

void Tetris::Release() {
	for (unsigned int i = 0; i < s_TileDivisionX; i++) {
		delete[] s_TetrisArray[i];
	}
	delete[] s_TetrisArray;
	delete s_InstBuffer;
}

void ReadConfigFile(const std::string & configFilePath) {
	prev::LuaScript script(configFilePath);

	s_TileDivisionX		= script.get<int>("Tetris_Config.TileDivision.X");
	s_TileDivisionY		= script.get<int>("Tetris_Config.TileDivision.Y");
	s_TileSizeX			= script.get<int>("Tetris_Config.TileSize.X");
	s_TileSizeY			= script.get<int>("Tetris_Config.TileSize.Y");
	s_UpdateInterval	= script.get<float>("Tetris_Config.UpdateInterval");
}
