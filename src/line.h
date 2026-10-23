#ifndef LINE_H
#define LINE_H

#include <memory>
#include <vector>

/* Символ: хранит позицию, значение и цвет, умеет рисоваться и стираться. */
class Symbol {
public:
    Symbol(int x, int y, char character, int colorCode);

    // Рисует символ на экране
    void draw() const;
    // Стирает символ с экрана
    void erase() const;
    int  x() const;
    int  y() const;

private:
    int  x_, y_;
    char character_;
    int  colorCode_;
};

/* Линия: сама управляет своим движением и скоростью.
   Менеджер лишь периодически вызывает update() с текущим временем. */
class Line {
public:
    // startTimeMs - момент (мс), в который линия должна начать движение
    Line(int length, int speed, int boardWidth, int boardHeight,
         bool epilepsy, long long startTimeMs);
    ~Line();

    // true, когда линия полностью доползла до конца экрана и стёрлась
    bool isFinished() const;

    // Выполняет все шаги, время которых уже наступило (без потоков и sleep)
    void update(long long currentTimeMs);

private:
    int       length_;
    int       boardWidth_, boardHeight_;
    int       axisExtent_;
    int       perpendicularLine_;
    int       step_;
    int       wholeLength_;
    bool      finished_;
    int       colorCode_;
    long long stepIntervalMs_;
    long long nextStepTimeMs_;
    std::vector<std::unique_ptr<Symbol>> body_;

    // Один шаг: рисует новую голову и стирает хвост
    void advanceOneStep();
    int  headPositionAt(int step) const;
    void coordsFor(int axisPosition, int step, int &x, int &y) const;
    static char randomGlyph();
};

#endif