#include "CCtrl.h"
#include <conio.h>

//HANDLE gOUTPUT;

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
	gAPI.OutputDebugPrintf("初始化。");
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

//游戏开始
#pragma region 初始化游戏世界
int CCtrl::Go(int index)
{
	byte bGame = 'G', key = 0, i = 0, dir = 0;
	CHARMAP tmap = { 0 };
	COORD xy = { 0,0 };
	if (de单人游戏闯关 == index) {
		PM->SetMap(1);
		this->_tanks = 3;
		this->cT[0].Init({ MAP_W / 2, MAP_H - 2 }, 100, 1, 'D', 'A', 0x04);
		tmap.TANKER = 1;
		PM->SetMap(cT[tmap.TANKER - 1].GetOldXY(), tmap, true);
		this->cT[1].Init({ 1, 1 }, 0, 1, 'D', 'A', 0x09);
		this->cT[2].Init({ MAP_W / 2 - 6, 1 }, 100, 2, 'S', 'B', 0x03);
		this->cT[3].Init({ MAP_W / 2 + 6, 1 }, 0, 2, 'S', 'B', 0x03);
		tmap.TANKER = 3;
		PM->SetMap(cT[tmap.TANKER - 1].GetOldXY(), tmap, true);
	}
#pragma endregion

//开始循环游戏
#pragma region 开始循环游戏
	PV->PrintMap();
	do
	{
		PV->PrintMap(cT[0]);
		tmap.TANKER = 0;
		for (byte i = 2; i < this->_tanks; i++) {
			PM->SetMap(cT[i].GetOldXY(), tmap, true);
			PV->PrintMap(cT[i], true);
		}
		//子弹流程
		for (byte i = 2; i < this->_tanks; i++) {
			if (!cT[i].IsAlive()) continue;
			tmap.TANKER = i + 1;
			do {
				dir = NULL;
				gAPI.RandDir(dir);
			} while (!cT[i].TryMove(dir));
			cT[i].Move();
			PM->SetMap(cT[i].GetOldXY(), tmap, true);
			PV->PrintMap(cT[i]);
		}
		key = gAPI.GetKey(); Sleep(100);		//通过API返回键值并延时

		switch (key) {
		case 'W':case 'A':case 'S':case 'D':	//方向键移动坦克1
			if (cT[0].TryMove(key) == true) {	//成功移动
				tmap.TANKER = 0;				//标识地图为坦克NULL
				PM->SetMap(cT[0].GetOldXY(), tmap, true);
				PV->PrintMap(cT[0], true);		//清除旧的坦克
				cT[0].Move();					//赋值新的坐标
				tmap.TANKER = 1;				//标识地图为玩家1
				PM->SetMap(cT[0].GetOldXY(), tmap, true);
				PV->PrintMap(cT[0]);			//打印新的坦克
			} break;
		case 'F':
			this->AddBullet(cT[0]);
			break;
		case 'I':case 'J':case 'K':case 'L':	//方向键移动坦克2
			break;
		case KEY_ESC: bGame = 0x00; break;
		default: break;
		}
	} while (bGame);

	memset(map, 0, MAP_H * MAP_W);				//单局游戏结束初始化地图
	return 0;
}

bool CCtrl::AddBullet(CTanker& cT)
{
	return true;
}
#pragma endregion
