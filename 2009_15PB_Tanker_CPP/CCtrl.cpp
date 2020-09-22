#include "CCtrl.h"
#include <conio.h>

//HANDLE gOUTPUT;

#pragma region 初始化游戏环境
CCtrl::CCtrl(COORD xy)
	:_maxXY(xy)
{
	PV = new CView(); PM = new CMap();
	if (!PV || !PM) {
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
		this->_tanks = 4;
		this->cT[0].Init({ MAP_W / 2, MAP_H - 2 }, 100, 1, 'D', 'A', 0x04);
		this->cT[1].Init({ 1, 1 }, 0, 1, 'D', 'A', 0x09);
		this->cT[2].Init({ MAP_W / 2 - 6, 1 }, 100, 2, 'S', 'B', deCol敌方速度型);
		this->cT[3].Init({ MAP_W / 2 + 6, 1 }, 100, 2, 'S', 'B', deCol敌方超重型);
		tmap.TANKER = 1;
		PM->SetMap(cT[tmap.TANKER - 1].GetOldXY(), tmap, true);
	}
#pragma endregion

//开始循环游戏
#pragma region 开始循环游戏
	PV->PrintMap();
	PV->PrintMap(cT[0]);
	do
	{

#pragma region AI控制逻辑段
		tmap.TANKER = 0;
		for (i = 2; i < this->_tanks; i++) {
			PM->SetMap(cT[i].GetOldXY(), tmap, true);
			PV->PrintMap(cT[i], true);
		}
		//子弹流程
		auto begin = _bullets.begin();
		while (begin != _bullets.end()) {
			MoveBullet(*begin, true); ++begin;
		}
		MoveTank();
		begin = _bullets.begin();
		while (begin != _bullets.end()) {
			if ((*begin).IsAlive()) {
				++begin;
			}else begin = _bullets.erase(begin);
		}
#pragma endregion
#pragma region 人工控制逻辑段
		key = gAPI.GetKey();					//通过API返回键值并延时

		switch (key) {
		case 'W':case 'A':case 'S':case 'D':	//方向键移动坦克1
			tmap.TANKER = 0;					//标识地图为坦克NULL
			PM->SetMap(cT[0].GetOldXY(), tmap, true);
			if (cT[0].TryMove(key) == true) {	//成功移动
				PV->PrintMap(cT[0], true);		//清除旧的坦克
				cT[0].Move();					//赋值新的坐标
				tmap.TANKER = 1;				//标识地图为玩家1
				PM->SetMap(cT[0].GetOldXY(), tmap, true);
				PV->PrintMap(cT[0]);			//打印新的坦克
			} else {
				tmap.TANKER = 1;				//标识地图为坦克1
				PM->SetMap(cT[0].GetOldXY(), tmap, true);
			} break;
		case 'F': this->AddBullet(0x01); break;
		case 'I':case 'J':case 'K':case 'L':	//方向键移动坦克2
			break;
		case KEY_ESC: bGame = 0x00; break;
		default: break;
		}
#pragma endregion
		Sleep(100);
	} while (bGame);
	memset(map, 0, MAP_H * MAP_W);				//单局游戏结束初始化地图
	return 0;
}
#pragma endregion

bool CCtrl::AddBullet(byte tid)
{
	CTanker& tank = this->cT[tid-1];
	CBullet att;
	if(att.Init(tank.GetOldXY(), tank.GetDir(false),tank.GetTid())){
		if (MoveBullet(att))
			_bullets.push_back(att);
	}
	return true;
}

bool CCtrl::MoveBullet(CBullet& att, bool clean)
{
	COORD xy = att.GetOldXY();
	CHARMAP tmap = map[xy.Y][xy.X];
	if (clean) {
		map[xy.Y][xy.X].Bullet = false;	//如果传入还原子弹，则重画
		if (tmap.Rever == true) {		//还原蓝色河流
			PV->PrintPoint(xy, deStr河流, deCol河流);
		}
		else if (tmap.Grass == true) {	//还原绿色草坪
			PV->PrintPoint(xy, deStr草坪, deCol草坪);
		}
		else {
			PV->PrintPoint(xy, deStr空地);
		}
	}
	if (att.TryMove()) {				//试图移动子弹
		xy = att.Move();				//如果成功，则移动
		tmap = map[xy.Y][xy.X];			//并刷新地图
		if (tmap.Rever == true) {		//河流则画蓝色的子弹
			PV->PrintPoint(xy, deStr子弹, deCol河流);
		}
		else if (tmap.Grass == true) {	//草坪则不重画

		}
		else if (tmap.Earth == true) {	//土墙则销毁
			att.SetBlood(0);			//销毁子弹
			tmap = { 0 };
			PM->SetMap(xy, tmap);		//重置地图
			PV->PrintPoint(xy, deStr空地);//画空地
			return false;
		}
		else {
			PV->PrintPoint(xy, deStr子弹);//画子弹
		}
	}
	else return false;		//如果试图移动失败，则返回假
	return true;
}

bool CCtrl::MoveTank()
{
	CHARMAP tmap = { 0 };
	byte dir, i;
	for (i = 2; i < this->_tanks; i++) {
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
	return false;
}

void CCtrl::PrintGInfo()
{

}

