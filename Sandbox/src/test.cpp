#include "pch.h"
#include <prev.h>

using namespace prev;

int main() {
	Log::ShouldShowLogOnTerminal(false);
	auto app = new Application();
	app->GetWindow().SetVSync(true);
	app->Run();
	delete app;
	return 0;
}