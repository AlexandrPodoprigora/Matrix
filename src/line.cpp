#include <cstdlib>

#include "line.h"
#include "console.h"

Line::Line(int length, int speed, int boardWidth, int boardHeight,
           bool epilepsy, long long startTimeMs)
    : length_(length), boardWidth_(boardWidth), boardHeight_(boardHeight),
      step_(0), wholeLength_(0), finished_(false),
      stepIntervalMs_(1000 / speed), nextStepTimeMs_(startTimeMs),
      body_(length) {
    colorCode_ = epilepsy ? Console::randomBrightColor() : Console::Green;

    // Линия движется по горизонтали: ось движения - ширина экрана,
    // а perpendicularLine_ - случайная строка, вдоль которой ползёт линия.
    // Отступаем на 1 от краёв, чтобы "извивы" (±1) не вылезали за экран.
    axisExtent_ = boardWidth_;
    perpendicularLine_ = (boardHeight_ > 2) ? 1 + std::rand() % (boardHeight_ - 2) : 0;
}

Line::~Line() {
    for (auto& symbol : body_) {
        if (symbol) symbol->erase();
    }
}

bool Line::isFinished() const { return finished_; }

void Line::update(long long currentTimeMs) {
    // while, а не if: если цикл менеджера запоздал, линия догонит свои шаги
    while (!finished_ && currentTimeMs >= nextStepTimeMs_) {
        advanceOneStep();
        nextStepTimeMs_ += stepIntervalMs_;
    }
}

void Line::advanceOneStep() {
    int slot = step_ % length_;

    if (body_[slot]) {
        body_[slot]->erase();
        body_[slot].reset();
    }

    int headPosition = headPositionAt(wholeLength_);
    if (headPosition >= 0 && headPosition < axisExtent_) {
        int x, y;
        coordsFor(headPosition, step_, x, y);
        body_[slot] = std::make_unique<Symbol>(x, y, randomGlyph(), colorCode_);
        body_[slot]->draw();
    }

    ++step_;
    if (step_ % 3 < 2) ++wholeLength_;
    if (wholeLength_ >= axisExtent_ + length_) finished_ = true;
}

int Line::headPositionAt(int wholeLength) const { return wholeLength; }

void Line::coordsFor(int axisPosition, int step, int &x, int &y) const {
    // Голова стартует у правого края (x = ширина - 1) и идёт к x = 0,
    // по вертикали "виляет" вокруг строки perpendicularLine_ (форма ромб).
    x = (boardWidth_ - 1) - axisPosition;
    switch (step % 3) {
        case 0:  y = perpendicularLine_;     break;
        case 1:  y = perpendicularLine_ + 1; break;
        default: y = perpendicularLine_ - 1; break;
    }
    if (y < 0) y = 0;
    if (y >= boardHeight_) y = boardHeight_ - 1;
}

char Line::randomGlyph() {
    static const char glyphs[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ*+-/=.";
    int count = static_cast<int>(sizeof(glyphs) - 1);
    return glyphs[std::rand() % count];
}

Symbol::Symbol(int x, int y, char character, int colorCode)
    : x_(x), y_(y), character_(character), colorCode_(colorCode) {}

void Symbol::draw() const  { Console::putChar(x_, y_, character_, colorCode_); }
void Symbol::erase() const { Console::eraseChar(x_, y_); }

int Symbol::x() const { return x_; }
int Symbol::y() const { return y_; }