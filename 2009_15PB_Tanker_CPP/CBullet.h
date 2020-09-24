#pragma once
#include "CTanker.h"
class CBullet : public CTanker  //�̳�̹����
{
public:
    /*��ʼ���ӵ�*/
    bool Init(COORD xy, byte dir, byte tid);

    /*��ͼ�ƶ��ӵ�*/
    bool TryMove(bool isRet = false);

    /*����ӵ�*/
    bool CheckBullet();

    /*�����ӵ�Ѫ������ʶ�Ƿ�����*/
    void SetBlood(int blood) {
        this->_blood = blood;
    };

    /*�����ӵ�����*/
    void SetDir(byte Dir) {
        switch (Dir) {
        case 'W': case 'A': case 'S': case 'D':  
            this->_dir = Dir; break;
        default: break;
        }
    };

    /*�����ӵ�Ѫ������ʶ�Ƿ�����*/
    void MinBlood() {
        --this->_blood;
    };
};
