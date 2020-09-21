#pragma once
#include "CTanker.h"
class CBullet :
    public CTanker
{
public:
    bool Init(COORD xy, byte dir, byte tid);
    bool TryMove(bool isRet);
    bool CheckBullet();
};
