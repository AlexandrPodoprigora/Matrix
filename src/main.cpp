#include <iostream>
#include <string>
#include <cctype>

#include "console.h"
#include "app_manager.h"

static bool parseIntInRange(const std::string& s, int lo, int hi, int& out) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit(static_cast<unsigned char>(c))) return false;
    try {
        int v = std::stoi(s);
        if (v < lo || v > hi) return false;
        out = v;
        return true;
    } catch (...) {
        return false;
    }
}

static bool parseEpilepsyFlag(const std::string& s, bool& out) {
    if (s.size() != 1) return false;
    char c = static_cast<char>(toupper(static_cast<unsigned char>(s[0])));
    if (c == 'Y') { out = true;  return true; }
    if (c == 'N') { out = false; return true; }
    return false;
}


static Params readParamsInteractively() {
    Params p;
    std::string line;

    while (true) {
        std::cout << "Введите скорость линии (1-30, симв/сек): ";
        std::getline(std::cin, line);
        if (parseIntInRange(line, 1, 30, p.speed)) break;
        std::cout << "Некорректное значение. Допустимый диапазон: 1-30.\n";
    }
    while (true) {
        std::cout << "Введите длину линии (1-30, символов): ";
        std::getline(std::cin, line);
        if (parseIntInRange(line, 1, 30, p.length)) break;
        std::cout << "Некорректное значение. Допустимый диапазон: 1-30.\n";
    }
    while (true) {
        std::cout << "Включить режим эпилепсии? (Y/N): ";
        std::getline(std::cin, line);
        if (parseEpilepsyFlag(line, p.epilepsy)) break;
        std::cout << "Некорректное значение. Введите Y или N.\n";
    }
    return p;
}

static bool readParamsFromCli(int argc, char** argv, Params& p) {
    if (argc != 4) return false;
    if (!parseIntInRange(argv[1], 1, 30, p.speed))  return false;
    if (!parseIntInRange(argv[2], 1, 30, p.length)) return false;
    if (!parseEpilepsyFlag(argv[3], p.epilepsy))    return false;
    return true;
}

static void printHelp(const char* progName) {
    std::cout <<
        "Matrix - лабораторная работа по ООП\n\n"
        "Использование:\n"
        "  " << progName << "                    запуск в диалоговом режиме\n"
        "  " << progName << " <скорость> <длина> <Y|N>   запуск в режиме CLI\n"
        "  " << progName << " --help | /?         вывод этой справки\n\n"
        "Параметры:\n"
        "  скорость   целое число 1..30   - скорость линии (симв/сек)\n"
        "  длина      целое число 1..30   - длина линии (символов)\n"
        "  Y|N                            - режим эпилепсии (вкл/выкл)\n\n"
        "Пример:\n"
        "  " << progName << " 23 8 Y\n";
}


int main(int argc, char** argv) {
    // Чтобы русский текст (исходники в UTF-8) нормально выводился в консоли Windows
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "/?")) {
        printHelp(argv[0]);
        return 0;
    }

    Params params;
    if (argc == 1) {
        params = readParamsInteractively();
    } else {
        if (!readParamsFromCli(argc, argv, params)) {
            std::cerr << "Ошибка: некорректные аргументы командной строки.\n\n";
            printHelp(argv[0]);
            return 1;
        }
    }

    AppManager app(params);
    app.run();

    return 0;
}
