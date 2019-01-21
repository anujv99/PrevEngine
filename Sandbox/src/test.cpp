#include "pch.h"
#include <prev.h>

using namespace prev;

int main() {
	Log::ShouldShowLogOnTerminal(false);
	PV_WARN("Using Prev Engine");
	auto app = new Application();
	app->Run();
	delete app;
	return 0;
}