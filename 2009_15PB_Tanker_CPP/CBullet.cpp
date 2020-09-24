#include "CBullet.h"

/*初始化子弹*/
bool CBullet::Init(COORD xy, byte dir, byte tid)
{
    this->_blood = 1;
    this->_oxy = { xy.X,xy.Y };
    this->_tid = tid;
    this->_dir = dir;
    this->TryMove(true);
    this->Move();
    return true;
}

/*试图移动子弹*/
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

/*检查子弹*/
bool CBullet::CheckBullet()
{
    if (this->_nxy.X == MAP_W ||
        this->_nxy.Y == MAP_H) {
        this->_blood = 0;
        return false;
    }else if (this->_nxy.X == -1
        || this->_nxy.Y == -1) {
        this->_blood = 0;
        return false;
    }
    CHARMAP tmap = map[_nxy.Y][_nxy.X];
    if (tmap.Wall == true) {
        this->_blood = 0;
        return false;
    }
    return true;
}
