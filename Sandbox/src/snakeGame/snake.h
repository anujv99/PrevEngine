#pragma once

#include <prev.h>

struct SnakeGame {
	static void CreateSnake(prev::Application * app, std::string configFilePath);
	static void ReleaseSnake();
};
