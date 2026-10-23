#include <iostream>
#include <string>
#include <cctype>

#include "console.h"
#include "app_manager.h"

// Границы допустимых значений числовых параметров
static const int MIN_PARAMETER_VALUE = 1;
static const int MAX_PARAMETER_VALUE = 30;

// Разбирает целое число из строки и проверяет диапазон
static bool parseIntInRange(const std::string& text, int low, int high, int& result) {
    if (text.empty()) return false;
    for (char symbol : text) {
        if (!isdigit(static_cast<unsigned char>(symbol))) return false;
    }
    try {
        int value = std::stoi(text);
        if (value < low || value > high) return false;
        result = value;
        return true;
    } catch (...) {
        return false;
    }
}

// Разбирает флаг режима эпилепсии: Y или N
static bool parseEpilepsyFlag(const std::string& text, bool& result) {
    if (text.size() != 1) return false;
    char symbol = static_cast<char>(toupper(static_cast<unsigned char>(text[0])));
    if (symbol == 'Y') { result = true;  return true; }
    if (symbol == 'N') { result = false; return true; }
    return false;
}

// Запрашивает число в диалоге, пока не будет введено корректное значение
static int readIntegerInteractively(const std::string& prompt) {
    std::string input;
    int value = 0;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        if (parseIntInRange(input, MIN_PARAMETER_VALUE, MAX_PARAMETER_VALUE, value)) return value;
        std::cout << "Некорректное значение. Допустимый диапазон: "
                  << MIN_PARAMETER_VALUE << "-" << MAX_PARAMETER_VALUE << ".\n";
    }
}

// Диалоговый режим ввода всех четырёх параметров
static Params readParamsInteractively() {
    Params params;
    params.frequency = readIntegerInteractively("Введите частоту генерации линий (1-30, линий/сек): ");
    params.speed     = readIntegerInteractively("Введите скорость линии (1-30, симв/сек): ");
    params.length    = readIntegerInteractively("Введите длину линии (1-30, символов): ");

    std::string input;
    while (true) {
        std::cout << "Включить режим эпилепсии? (Y/N): ";
        std::getline(std::cin, input);
        if (parseEpilepsyFlag(input, params.epilepsy)) break;
        std::cout << "Некорректное значение. Введите Y или N.\n";
    }
    return params;
}

// Режим командной строки: <частота> <скорость> <длина> <Y|N>
static bool readParamsFromCli(int argc, char** argv, Params& params) {
    if (argc != 5) return false;
    if (!parseIntInRange(argv[1], MIN_PARAMETER_VALUE, MAX_PARAMETER_VALUE, params.frequency)) return false;
    if (!parseIntInRange(argv[2], MIN_PARAMETER_VALUE, MAX_PARAMETER_VALUE, params.speed))     return false;
    if (!parseIntInRange(argv[3], MIN_PARAMETER_VALUE, MAX_PARAMETER_VALUE, params.length))    return false;
    if (!parseEpilepsyFlag(argv[4], params.epilepsy)) return false;
    return true;
}

// Выводит справку по запуску
static void printHelp(const char* programName) {
    std::cout <<
        "Matrix - лабораторная работа по ООП\n\n"
        "Использование:\n"
        "  " << programName << "                                    запуск в диалоговом режиме\n"
        "  " << programName << " <частота> <скорость> <длина> <Y|N>  запуск в режиме CLI\n"
        "  " << programName << " --help | /?                        вывод этой справки\n\n"
        "Параметры:\n"
        "  частота    целое число 1..30   - появление линий (линий/сек)\n"
        "  скорость   целое число 1..30   - скорость линии (симв/сек)\n"
        "  длина      целое число 1..30   - длина линии (символов)\n"
        "  Y|N                            - режим эпилепсии (вкл/выкл)\n\n"
        "Пример:\n"
        "  " << programName << " 3 23 8 Y\n";
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
    } else if (!readParamsFromCli(argc, argv, params)) {
        std::cerr << "Ошибка: некорректные аргументы командной строки.\n\n";
        printHelp(argv[0]);
        return 1;
    }

    AppManager app(params);
    app.run();
    return 0;
}