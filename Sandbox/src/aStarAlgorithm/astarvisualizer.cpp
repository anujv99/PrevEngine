#include "pch.h"
#include "astarvisualizer.h"

using namespace prev;

void AStarVisualizer::ShowPath(std::vector<AStarNode *> path) {
	auto level = LevelManager::CreateEmptyLevel("AStarLevel");
	LevelManager::StartLevel(level);

	auto levelLayer = level->CreateNewLevelLayer(false);

	const Shader * aStarShader = ShaderManager::LoadShader("AStarShader",
		(Window::GetExePath() + "../../../Sandbox/res/Shaders/shader.vert").c_str(),
		(Window::GetExePath() + "../../../Sandbox/res/Shaders/shader.frag").c_str());

	levelLayer->SetShader(aStarShader);

	Tiles tile(20, 20);

	for (auto node : path) {
		auto entity = levelLayer->GetEntityXLayer()->entities.create();
		entity.assign<components::Position>(tile.GetTilePosition(node->GetPos().x, node->GetPos().y));
		entity.assign<components::Scale>(tile.GetTileSize());
		entity.assign<components::Renderable>();
	}

	levelLayer->GetEntityXLayer()->AddSystem<systems::RenderSystem>();
}

AStarVisualizer::~AStarVisualizer() {

}
