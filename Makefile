# Сборка под Windows компилятором MinGW (g++):  mingw32-make
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS  = -static
TARGET   = matrix.exe

SRCS     = src/main.cpp src/console.cpp src/app_manager.cpp src/line.cpp
OBJS     = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q src\*.o $(TARGET) 2>nul || rm -f $(OBJS) $(TARGET)

.PHONY: all clean
