#include "app_manager.h"
#include "console.h"
#include "line.h"


AppManager::AppManager(const Params& params) : params_(params) {}

void AppManager::run() {
	Console::init();
	int width  = Console::getWidth();
	int height = Console::getHeight();
	int delayMs = 1000 / params_.speed;

	bool exitRequested = false;
	while (!exitRequested) {
		Line* line = new Line(params_.length, width, height, params_.epilepsy);

		while (!line->isFinished()) {
			if (Console::escPressed()) { exitRequested = true; break; }
			line->advance();
			Console::sleepMs(delayMs);
		}
		delete line;
	}

	Console::shutdown();
}
