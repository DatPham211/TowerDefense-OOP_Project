#pragma once
#include "cenemy.h"
#include "ctower.h"

#include <iostream>

using namespace std;

class cmap
{
private:
    cenemy _ce;
    ctower _ctw;
    cpoint _m[cpoint::MAP_ROW][cpoint::MAP_COL];
public:
    cmap();

    void resetMapData();
    void makeMapData();
    // void drawMap();

    cenemy& getEnemy() { return _ce; }
    ctower& getTower() { return _ctw; }
    cpoint(&getMap())[cpoint::MAP_ROW][cpoint::MAP_COL]{ return _m; }
};