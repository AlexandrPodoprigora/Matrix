#include <algorithm>
#include <cstdlib>

#include "app_manager.h"
#include "console.h"

AppManager::AppManager(const Params& params) : params_(params) {}

void AppManager::run() {
    Console::init();
    boardWidth_  = Console::getWidth();
    boardHeight_ = Console::getHeight();

    planSpawnWindow(Console::currentTimeMs());

    while (!Console::escPressed()) {
        long long now = Console::currentTimeMs();

        // Прошла секунда - планируем появление линий на следующую
        if (now - windowStartMs_ >= WINDOW_MS) {
            planSpawnWindow(windowStartMs_ + WINDOW_MS);
        }

        spawnDueLines(now);
        updateLines(now);
        removeFinishedLines();
        Console::sleepMs(1);
    }

    lines_.clear();
    Console::shutdown();
}

void AppManager::planSpawnWindow(long long windowStartMs) {
    windowStartMs_ = windowStartMs;
    nextSpawnIndex_ = 0;
    spawnTimes_.clear();

    for (int i = 0; i < params_.frequency; ++i) {
        spawnTimes_.push_back(windowStartMs + std::rand() % WINDOW_MS);
    }
    std::sort(spawnTimes_.begin(), spawnTimes_.end());
}

void AppManager::spawnDueLines(long long currentTimeMs) {
    while (nextSpawnIndex_ < spawnTimes_.size() &&
           spawnTimes_[nextSpawnIndex_] <= currentTimeMs) {
        lines_.push_back(std::make_unique<Line>(
            params_.length, params_.speed, boardWidth_, boardHeight_,
            params_.epilepsy, spawnTimes_[nextSpawnIndex_]));
        ++nextSpawnIndex_;
    }
}

void AppManager::updateLines(long long currentTimeMs) {
    for (auto& line : lines_) {
        line->update(currentTimeMs);
    }
}

void AppManager::removeFinishedLines() {
    lines_.erase(
        std::remove_if(lines_.begin(), lines_.end(),
                       [](const std::unique_ptr<Line>& line) { return line->isFinished(); }),
        lines_.end());
}