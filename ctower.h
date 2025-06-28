#pragma once
#include "cpoint.h"
#include "cbullet.h"
#include "cenemy.h"

class ctower
{
    cpoint _location;
    cbullet _cb;
public:
    ctower();
    cpoint getLocation() const;
    void setLocation(const cpoint& tl);
    void setMapForBullet(cpoint map[][cpoint::MAP_COL]);
    void calcPathBullet();
    cbullet& getBullet();
};
