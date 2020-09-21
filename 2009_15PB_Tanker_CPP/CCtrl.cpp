#include "CCtrl.h"
#include <conio.h>

//CHARMAP map[MAP_H][MAP_W];
HANDLE gOUTPUT;
//PGAMEINFO gGINFO;

#pragma region ��ʼ����Ϸ����
CCtrl::CCtrl(COORD xy)
	:_maxXY(xy)
{
	PV = new CView();
	if (!PV) {
		printf_s("��ʼ����ͼ����\n");
		exit(1);
	}
	PM = new CMap();
	if (!PV) {
		printf_s("��ʼ����ͼ����\n");
		exit(1);
	}
	
	gOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
}

CCtrl::~CCtrl()
{
	//�ͷ���ͼ��&��ͼ��
	delete PV;
	delete PM;
}

bool CCtrl::InitCMD()
{
	//��ʼ�����ڹ�ꡢ��С
	if (PV->InitWindow(_maxXY, false) == false)
		return false;
	return true;
}
#pragma endregion
#pragma region ��ʼ����Ϸ����
//��Ϸ��ʼ
int CCtrl::Go(int index)
{
	byte bGame = 'G', key = 0;
	if (de������Ϸ���� == index) {
		PM->SetMap(1);
		this->_tanks = 3;
		this->cT[0].Init({ MAP_W / 2, MAP_H - 2 }, 1, 'D', 'A', 0x04);
		this->cT[1].Init({ 1, 1 }, 1, 'D', 'A', 0x09);
		this->cT[2].Init({ MAP_W / 2 - 3, 1 }, 2, 'S', 'B', 0x03);
		PV->PrintMap();
		do
		{
			PV->PrintMap(cT[0]);
			for (byte i = 2; i < this->_tanks; i++)
				PV->PrintMap(cT[i]);
			switch (key = _getch())
			{
			case 'W':case 'A':case 'S':case 'D':
				if (cT[0].TryMove(key) == true) {	//�ɹ��ƶ�
					PV->PrintMap(cT[0]);
				}break;
			case KEY_ESC: bGame = 0x00; break;
			default:
				break;
			}
		} while (bGame);
	}
	return 0;
}

#pragma endregion