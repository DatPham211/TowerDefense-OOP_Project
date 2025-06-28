#pragma once
#include "cpoint.h"
#include "cenemy.h" // Make sure to include the enemy header

class cbullet
{
private:
    int _n, _speed;
    cpoint _p[cpoint::MAP_ROW * cpoint::MAP_COL];
    cpoint _m[cpoint::MAP_ROW][cpoint::MAP_COL];
    cpoint _curr;

    bool active = false;
    int targetIndex = -1;
public:
    cbullet();

    // logic
    void updateMap(int i, int j, cpoint v);
    int queryCFromRowCol(int row, int col) const;
    int calcPathBullet(const cpoint& tower);

    void trackEnemy(const cenemy& enemy); // Move bullet one step toward the enemy's current position
    bool checkCollision(const cenemy& enemy) const; // Bullet collision detection

    // getters
    cpoint getCurr() const;
    cpoint* getP();
    int getSpeed() const;
    int getN() const;
    int getTargetIndex() const;
    bool isActive() const;

    // setter
    void setCurr(const cpoint& tcurr);
    void setN(int tn);
    void setSpeed(int tspeed);
    void setTargetIdx(int idx);
    void activate();
    void deactivate();
};
