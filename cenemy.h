#pragma once
#include "cpoint.h"

#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

class cenemy
{
private:
    int _speed;
    cpoint _start, _end, _curr;
    cpoint _p[cpoint::MAP_ROW * cpoint::MAP_COL];
    int _pathLength;
    int dd[4], dc[4];

    // runtime properties
    int currentTarget = 1;
    bool reachEnd = false;
    int hitCount = 0;
public:
    cenemy();
    cenemy(cpoint tstart, cpoint tend, cpoint tcurr);

    // runtime process
    void findPath(cpoint a[][cpoint::MAP_COL], cpoint s, cpoint e);
    void move(float dt);
    void onHit();
    bool isDead() const;
    bool isReachedEnd() const;

    void setCurr(const cpoint& tcurr);
    cpoint getCurr() const;

    // path access
    cpoint* getP();
    int getPathLength() const;
    int getCurrentTarget() const;
    void setCurrentTarget(int);

    // getters, setters
    cpoint getStart() const;
    cpoint getEnd() const;
    int getSpeed() const;
    void setStart(const cpoint& tstart);
    void setEnd(const cpoint& tend);
    void setSpeed(int tspeed);
private:
    void calcPath(int a[][cpoint::MAP_COL], int n, cpoint s, cpoint e);
};