#ifndef CONSOLE_H
#define CONSOLE_H

#include <windows.h>

class Console {
public:
    // Атрибуты цвета текста консоли Windows (яркие варианты)
    enum Color {
        Black   = 0,
        Red     = FOREGROUND_RED   | FOREGROUND_INTENSITY,
        Green   = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        Yellow  = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        Blue    = FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
        Magenta = FOREGROUND_RED   | FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
        Cyan    = FOREGROUND_GREEN | FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
        White   = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    };

    static void init();
    static void shutdown();
    static void clearScreen();
    static void hideCursor();
    static void showCursor();
    static void gotoXY(int x, int y);
    static void setColor(int colorCode);
    static void resetColor();
    static void putChar(int x, int y, char ch, int colorCode);
    static void eraseChar(int x, int y);
    static void sleepMs(int ms);
    static bool escPressed();
    static int getWidth();
    static int getHeight();
    static int randomBrightColor();

private:
    static int getSize(bool width);
    static COORD toBuffer(int x, int y);   // координаты окна -> координаты буфера

    static inline HANDLE hOut_          = INVALID_HANDLE_VALUE;
    static inline WORD   origAttr_      = Color::White;
    static inline CONSOLE_CURSOR_INFO origCursor_{};
};

#endif
