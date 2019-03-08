#pragma once

#include <prev.h>

struct Tetris {
	static void CreateTetris(prev::Application * app, const std::string & configFilePath);
	static void Release();
};