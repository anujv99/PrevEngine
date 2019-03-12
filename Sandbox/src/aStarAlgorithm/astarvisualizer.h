#pragma once

#include "astart.h"

class AStarVisualizer {
public:
	static void ShowPath(std::vector<AStarNode *> path);
	~AStarVisualizer();
};
