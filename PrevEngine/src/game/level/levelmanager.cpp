#include "pch.h"
#include "levelmanager.h"
#include "application.h"

namespace prev {

	std::map<std::string, Level *> LevelManager::m_Levels;
	Level * LevelManager::m_ActiveLevel = nullptr;
	Application * applicationInstance = nullptr;

	void LevelManager::Init() {
		applicationInstance = Application::GetApplicationInstance();
	}

	void LevelManager::Release() {
		for (auto level : m_Levels) {
			applicationInstance->PopLayer(level.second);
		}
		for (auto level : m_Levels) {
			delete level.second;
		}
	}

	Level * LevelManager::CreateEmptyLevel(std::string levelName) {
		if (m_Levels.find(levelName) != m_Levels.end()) {
			PV_ASSERT(false, "Level with name (%s) already exists", levelName.c_str());
			return nullptr;
		}

		Level * level = new Level();
		m_Levels.insert(std::pair<std::string, Level *>(levelName, level));
		return m_Levels[levelName];
	}

	void LevelManager::StartLevel(std::string levelName) {
		if (m_Levels.find(levelName) != m_Levels.end()) {
			PV_ASSERT(false, "Level with name (%s) already exists", levelName.c_str());
		}
		StopActiveLevel();
		applicationInstance->PushLayer(m_Levels[levelName]);
	}

	void LevelManager::StartLevel(Level * level) {
		PV_ASSERT(level, "Invalid level");
		StopActiveLevel();
		applicationInstance->PushLayer(level);
	}

	void LevelManager::StopActiveLevel() {
		if (m_ActiveLevel != nullptr) {
			PV_CORE_WARN("Stopping previously started level");
			applicationInstance->PopLayer(m_ActiveLevel);
			m_ActiveLevel = nullptr;
		}
	}

	

}