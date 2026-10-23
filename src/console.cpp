#include <chrono>
#include <cstdlib>
#include <ctime>
#include <conio.h>

#include "console.h"


void Console::init() {
    hOut_ = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(hOut_, &info)) origAttr_ = info.wAttributes;
    GetConsoleCursorInfo(hOut_, &origCursor_);

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    hideCursor();
    clearScreen();
}

void Console::shutdown() {
    clearScreen();
    resetColor();
    showCursor();
    gotoXY(0, 0);
}

COORD Console::toBuffer(int x, int y) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    SHORT left = 0, top = 0;
    if (GetConsoleScreenBufferInfo(hOut_, &info)) {
        left = info.srWindow.Left;
        top  = info.srWindow.Top;
    }
    COORD c;
    c.X = static_cast<SHORT>(left + x);
    c.Y = static_cast<SHORT>(top + y);
    return c;
}

void Console::clearScreen() {
    int w = getWidth(), h = getHeight();
    DWORD written;
    for (int y = 0; y < h; ++y) {
        COORD c = toBuffer(0, y);
        FillConsoleOutputCharacterA(hOut_, ' ', w, c, &written);
        FillConsoleOutputAttribute(hOut_, origAttr_, w, c, &written);
    }
    gotoXY(0, 0);
}

void Console::hideCursor() {
    CONSOLE_CURSOR_INFO ci = origCursor_;
    if (ci.dwSize == 0) ci.dwSize = 25;
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(hOut_, &ci);
}

void Console::showCursor() {
    CONSOLE_CURSOR_INFO ci = origCursor_;
    if (ci.dwSize == 0) ci.dwSize = 25;
    ci.bVisible = TRUE;
    SetConsoleCursorInfo(hOut_, &ci);
}

void Console::gotoXY(int x, int y) {
    SetConsoleCursorPosition(hOut_, toBuffer(x, y));
}

void Console::setColor(int colorCode) {
    SetConsoleTextAttribute(hOut_, static_cast<WORD>(colorCode));
}

void Console::resetColor() {
    SetConsoleTextAttribute(hOut_, origAttr_);
}

// Пишем символ прямо в буфер консоли: курсор не двигается,
// и экран не прокручивается даже в правом нижнем углу.
void Console::putChar(int x, int y, char ch, int colorCode) {
    if (x < 0 || y < 0 || x >= getWidth() || y >= getHeight()) return;
    COORD c = toBuffer(x, y);
    DWORD written;
    WORD attr = static_cast<WORD>(colorCode);
    WriteConsoleOutputAttribute(hOut_, &attr, 1, c, &written);
    WriteConsoleOutputCharacterA(hOut_, &ch, 1, c, &written);
}

void Console::eraseChar(int x, int y) {
    putChar(x, y, ' ', origAttr_);
}

void Console::sleepMs(int ms) {
    Sleep(static_cast<DWORD>(ms));
}

long long Console::currentTimeMs() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

bool Console::escPressed() {
    while (_kbhit()) {
        int c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }   // стрелки, F-клавиши
        if (c == 27) return true;
    }
    return false;
}

int Console::getWidth()  { return getSize(true); }
int Console::getHeight() { return getSize(false); }

int Console::randomBrightColor() {
    static const int palette[] = { Red, Yellow, Blue, Magenta, Cyan, White };
    return palette[std::rand() % 6];
}

int Console::getSize(bool width) {
    int w = 80, h = 25;
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(hOut_, &info)) {
        w = info.srWindow.Right  - info.srWindow.Left + 1;
        h = info.srWindow.Bottom - info.srWindow.Top  + 1;
    }
    return width ? w : h;
}