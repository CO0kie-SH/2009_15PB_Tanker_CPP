#include "CBullet.h"

bool CBullet::Init(COORD xy, byte dir, byte tid)
{
    this->_blood = 1;
    this->_oxy = { xy.X,xy.Y };
    this->_tid = tid;
    this->_dir = dir;
    this->TryMove(true);
    this->Move();
    this->TryMove(true);
    this->Move();
    return CheckBullet();
}

bool CBullet::TryMove(bool isRet)
{
    this->_nxy = { this->_oxy.X,this->_oxy.Y };
    switch (this->_dir) {
    case 'W': --this->_nxy.Y; break;
    case 'A': --this->_nxy.X; break;
    case 'S': ++this->_nxy.Y; break;
    case 'D': ++this->_nxy.X; break;
    default: break; }
    if (isRet) return true;
    return CheckBullet();
}

bool CBullet::CheckBullet()
{
    if (this->_oxy.X == MAP_W) {
        this->_blood = 0;
        return false;
    }
    CHARMAP tmap = map[_oxy.Y][_oxy.X];
    if (tmap.Wall == true)
        return false;
    return true;
}
