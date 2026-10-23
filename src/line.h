#ifndef LINE_H
#define LINE_H

class Symbol {
public:
    Symbol(int x, int y, char ch, int colorCode);
    void draw() const;
    void erase() const;
    int  x() const;
    int  y() const;

private:
    int  x_, y_;
    char ch_;
    int  colorCode_;
};

class Line {
public:
    Line(int length, int boardWidth, int boardHeight, bool epilepsy);
    ~Line();
    bool isFinished() const;
    void advance();

private:
    int        length_;
    int        boardWidth_, boardHeight_;
    int        axisExtent_;
    int        perp_; 
    int        step_;
    int        wholeLength_;
    bool       finished_;
    int        colorCode_;
    Symbol**   body_; 

    int headPositionAt(int step) const;
    void coordsFor(int axisPos, int step, int &x, int &y) const;
    static char randomGlyph();
};

#endif
