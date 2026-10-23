#include <cstdlib>

#include "line.h"
#include "console.h"

Line::Line(int length, int boardWidth, int boardHeight, bool epilepsy): 
          length_(length), boardWidth_(boardWidth), boardHeight_(boardHeight),
          step_(0), wholeLength_(0), finished_(false) {
	body_ = new Symbol*[length_];
	for (int i = 0; i < length_; ++i) body_[i] = nullptr;

	colorCode_ = epilepsy ? Console::randomBrightColor() : Console::Green;

	// Линия движется по горизонтали: ось движения - ширина экрана,
	// а perp_ - случайная строка, вдоль которой ползёт змейка.
	// Отступаем на 1 от краёв, чтобы "извивы" (perp_ ± 1) не вылезали за экран.
	axisExtent_ = boardWidth_;
	perp_ = (boardHeight_ > 2) ? 1 + std::rand() % (boardHeight_ - 2) : 0;
}

Line::~Line() {
    for (int i = 0; i < length_; ++i) {
        if (body_[i]) {
            body_[i]->erase();
            delete body_[i];
            body_[i] = nullptr;
        }
    }
    delete[] body_;
}

bool Line::isFinished() const { return finished_; }

void Line::advance() {
    int slot = step_ % length_;

    if (body_[slot]) {
        body_[slot]->erase();
        delete body_[slot];
        body_[slot] = nullptr;
    }

    int headPos = headPositionAt(wholeLength_);
    if (headPos >= 0 && headPos < axisExtent_) {
        int x, y;
        coordsFor(headPos, step_, x, y);
        char ch = randomGlyph();
        body_[slot] = new Symbol(x, y, ch, colorCode_);
        body_[slot]->draw();
    }

    ++step_;
    if (step_ % 3 < 2) ++wholeLength_;
    if (wholeLength_ >= axisExtent_ + length_) finished_ = true;
}

int Line::headPositionAt(int wholeLength) const { return wholeLength; }

void Line::coordsFor(int axisPos, int step, int &x, int &y) const {
    // Голова стартует у правого края (x = ширина - 1) и идёт к x = 0,
    // по вертикали "виляет" вокруг строки perp_.
    x = (boardWidth_ - 1) - axisPos;
    int off = step % 3;
    switch (off) {
        case 0:  y = perp_;     break;
        case 1:  y = perp_ + 1; break;
        default: y = perp_ - 1; break;
    }
    if (y < 0) y = 0;
    if (y >= boardHeight_) y = boardHeight_ - 1;
}

char Line::randomGlyph() {
    static const char glyphs[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ*+-/=.";
    int n = static_cast<int>(sizeof(glyphs) - 1);
    return glyphs[std::rand() % n];
}

Symbol::Symbol(int x, int y, char ch, int colorCode)
	: x_(x), y_(y), ch_(ch), colorCode_(colorCode) {};

void Symbol::draw() const  { Console::putChar(x_, y_, ch_, colorCode_); };
void Symbol::erase() const { Console::eraseChar(x_, y_); };

int  Symbol::x() const { return x_; }
int  Symbol::y() const { return y_; }
