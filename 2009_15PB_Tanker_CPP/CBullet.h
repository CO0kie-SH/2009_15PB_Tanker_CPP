#pragma once
#include "CTanker.h"
class CBullet : public CTanker  //继承坦克类
{
public:
    /*初始化子弹*/
    bool Init(COORD xy, byte dir, byte tid);

    /*试图移动子弹*/
    bool TryMove(bool isRet = false);

    /*检查子弹*/
    bool CheckBullet();

    /*设置子弹血量，标识是否销毁*/
    void SetBlood(int blood) {
        this->_blood = blood;
    };

    /*设置子弹方向*/
    void SetDir(byte Dir) {
        switch (Dir) {
        case 'W': case 'A': case 'S': case 'D':  
            this->_dir = Dir; break;
        default: break;
        }
    };

    /*设置子弹血量，标识是否销毁*/
    void MinBlood() {
        --this->_blood;
    };
};
