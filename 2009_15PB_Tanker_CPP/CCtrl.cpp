#include "CCtrl.h"
//CHARMAP map[MAP_H][MAP_W];
HANDLE gOUTPUT;
//PGAMEINFO gGINFO;

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
	this->cT[1].init({ 1, 1 }, 1);
	this->cT[2].init({ 1, 10 }, 2);
	gOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
}

CCtrl::~CCtrl()
{
	delete PV;
	delete PM;
}

bool CCtrl::InitCMD()
{
	PV->InitWindow(_maxXY, false);
	PV->PrintTank(cT[1]);
	return true;
}
