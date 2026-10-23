@echo off
chcp 65001 >nul
g++ -std=c++17 -Wall -Wextra -O2 -static -o matrix.exe src\main.cpp src\console.cpp src\app_manager.cpp src\line.cpp
if errorlevel 1 (echo Build failed & pause & exit /b 1)
echo Build OK: matrix.exe
