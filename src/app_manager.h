#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <cstddef>
#include <memory>
#include <vector>

#include "line.h"

// Параметры запуска программы
struct Params {
    int  frequency = 3;     // линий в секунду (1-30)
    int  speed     = 15;    // символов в секунду (1-30)
    int  length    = 15;    // длина линии в символах (1-30)
    bool epilepsy  = false; // режим эпилепсии
};

/* Менеджер приложения: планирует появление линий, координирует их
   движение в одном потоке и удаляет завершившиеся. */
class AppManager {
public:
    explicit AppManager(const Params& params);

    // Главный цикл; завершается по нажатию Esc
    void run();

private:
    static constexpr long long WINDOW_MS = 1000;

    Params params_;
    int    boardWidth_  = 0;
    int    boardHeight_ = 0;

    std::vector<std::unique_ptr<Line>> lines_;

    // Абсолютные моменты (мс) появления линий в текущей секунде, по возрастанию
    std::vector<long long> spawnTimes_;
    std::size_t            nextSpawnIndex_ = 0;
    long long              windowStartMs_  = 0;

    // Генерирует frequency случайных моментов появления внутри секунды
    void planSpawnWindow(long long windowStartMs);
    // Создаёт линии, чьё время появления наступило
    void spawnDueLines(long long currentTimeMs);
    void updateLines(long long currentTimeMs);
    void removeFinishedLines();
};

#endif