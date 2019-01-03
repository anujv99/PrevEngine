#include "pch.h"
#include <gl/GL.h>
#include <src/prev.h>

using namespace prev;

int main() {
	auto app = new Application();
	app->Run();
	delete app;
	return 0;
}