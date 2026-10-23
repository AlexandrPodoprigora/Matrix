# Laboratornye_OOP — лаба 1 (Windows-версия)

Разноцветные "змейки" из символов, которые ползут **справа налево** по окну консоли Windows.
Выход — клавиша **Esc**.

## Запуск
Готовый `matrix.exe` лежит в папке. Запускай из `cmd` или PowerShell:

```
matrix.exe                диалоговый режим
matrix.exe 20 10 Y        скорость 20, длина 10, режим эпилепсии
matrix.exe --help         справка
```

## Сборка
- **MinGW / MSYS2:** `build.bat` или `mingw32-make`
- **Visual Studio:** создать пустой консольный проект C++17, добавить все файлы из `src/`, собрать.

## Что поменялось относительно Linux-версии
- `console.cpp/.h` переписаны на WinAPI: `GetConsoleScreenBufferInfo`, `WriteConsoleOutputCharacterA`,
  `SetConsoleCursorInfo`, `_kbhit/_getch` вместо `termios`, `ioctl` и ANSI-кодов.
- Цвета — атрибуты консоли Windows (`FOREGROUND_*`).
- `line.cpp`: ось движения — ширина экрана, голова стартует у правого края и идёт к левому,
  "виляя" по строкам `perp ± 1`.
- `main.cpp`: включена кодировка UTF-8 для русского текста.
