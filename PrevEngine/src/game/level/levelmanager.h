#pragma once

#include "level.h"

namespace prev {

	class LevelManager {
	public:
		static void Init();
		static void Release();
		static Level * CreateEmptyLevel(std::string levelName);
		static void StartLevel(std::string levelName);
		static void StartLevel(Level * level);
		static void StopActiveLevel();
	public:
		inline static Level * GetActiveLevel() { PV_ASSERT(m_ActiveLevel, "no active level found"); return m_ActiveLevel; }
	private:
		static std::map<std::string, Level *> m_Levels;
		static Level * m_ActiveLevel;
	};

}