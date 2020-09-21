#include "CCtrl.h"
#include <conio.h>

//CHARMAP map[MAP_H][MAP_W];
HANDLE gOUTPUT;
//PGAMEINFO gGINFO;

#pragma region 初始化游戏环境
CCtrl::CCtrl(COORD xy)
	:_maxXY(xy)
{
	PV = new CView();
	if (!PV) {
		printf_s("初始化视图错误。\n");
		exit(1);
	}
	PM = new CMap();
	if (!PV) {
		printf_s("初始化视图错误。\n");
		exit(1);
	}
	
	gOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
}

CCtrl::~CCtrl()
{
	//释放视图类&地图类
	delete PV;
	delete PM;
}

bool CCtrl::InitCMD()
{
	//初始化窗口光标、大小
	if (PV->InitWindow(_maxXY, false) == false)
		return false;
	return true;
}
#pragma endregion
#pragma region 初始化游戏世界
//游戏开始
int CCtrl::Go(int index)
{
	byte bGame = 'G', key = 0;
	if (de单人游戏闯关 == index) {
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
				if (cT[0].TryMove(key) == true) {	//成功移动
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