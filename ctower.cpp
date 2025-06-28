#include "ctower.h"

ctower::ctower()
{
    _location = cpoint(0, 0, 0);
}

// getters setters
cbullet& ctower::getBullet()
{
    return _cb;
}
cpoint ctower::getLocation() const
{
    return _location;
}
void ctower::setLocation(const cpoint& tl)
{
    _location = tl;
}

// just call once after level loads
void ctower::setMapForBullet(cpoint map[][cpoint::MAP_COL])
{
    for (int i = 0; i < cpoint::MAP_ROW; i++)
        for (int j = 0; j < cpoint::MAP_COL; j++)
            _cb.updateMap(i, j, map[i][j]);
}

void ctower::calcPathBullet()
{
    _cb.setTargetIdx(1);
    _cb.activate();
    _cb.calcPathBullet(_location);
}