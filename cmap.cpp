#include "cmap.h"

cmap::cmap() {
    resetMapData();
    makeMapData();
}
void cmap::resetMapData() {
    for (int i = 0; i < cpoint::MAP_ROW; i++)
        for (int j = 0; j < cpoint::MAP_COL; j++)
            _m[i][j] = cpoint(i, j, -1); // row, col, type
}

void cmap::makeMapData() {
    // Bạn có thể thay thế map gốc này bằng map bạn tự design
    int map[10][15] = {
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, 0, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, 0, -1, -1, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    };
    for (int i = 0; i < cpoint::MAP_ROW; i++)
        for (int j = 0; j < cpoint::MAP_COL; j++)
            _m[i][j] = cpoint(i, j, map[i][j]);

    // Set vị trí start, end, curr cho enemy
    _ce.setStart(_m[5][0]);
    _ce.setEnd(_m[4][14]);
    _ce.setCurr(_m[5][0]);
    _ce.findPath(_m, _ce.getStart(), _ce.getEnd());

    // Set vị trí tower và map cho bullet
    _ctw.setLocation(_m[5][0]);
    _ctw.setMapForBullet(_m);
    _ctw.calcPathBullet();
}

/*
void cmap::drawMap() {
    for (int i = 0; i < cpoint::MAP_ROW; i++) {
        for (int j = 0; j < cpoint::MAP_COL; j++)
            cout << (_m[i][j].getC() == -1 ? '+' : (_m[i][j].getC() == 0 ? '.' : ' '));
        cout << '\n';
    }
}
*/