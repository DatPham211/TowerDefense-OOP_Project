#pragma once
#include <cmath>

class cpoint
{
public:
    static const int MAP_ROW = 10;
    static const int MAP_COL = 15;
    static const int TILE_SIZE = 80;

private:
    int row, col, c;

public:
    cpoint();
    cpoint(int trow, int tcol, int tc);

    int getRow() const;
    int getCol() const;
    int getC() const;
    void setC(int tc);

    int getPixelX() const;
    int getPixelY() const;

    void setPixelX(int);
    void setPixelY(int);

    static cpoint fromXYToRowCol(int x, int y, int c = 0); // find at which row/col (tile) are now
    static cpoint fromRowColToXY(int row, int col, int c = 0);
};
