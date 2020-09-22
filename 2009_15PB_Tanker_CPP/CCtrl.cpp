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
	unsigned int msecond = 0;
	if (de单人游戏闯关 == index) {		//初始化单人游戏
		PM->SetMap(1);
		gGINFO.player = 1;
		this->_tanks = 4;
		this->cT[0].Init({ MAP_W / 2, MAP_H - 2 }, 100,
			1, 'D', 'A', 0x04);
		this->cT[1].Init({ 1, 1 }, 0, 2, 'D', 'A', 0x09);
		this->cT[2].Init({ MAP_W / 2 - 6, 1 }, 100,
			3, 'S', 'B', deCol敌方速度型, 100);
		this->cT[3].Init({ MAP_W / 2 + 6, 1 }, 100,
			4, 'S', 'B', deCol敌方超重型, 300, 51);
		tmap.TANKER = 1;
		PM->SetMap(cT[tmap.TANKER - 1].GetOldXY(), tmap, true);
	}
	else if (de双人游戏闯关 == index) {	//初始化双人游戏
		PM->SetMap(1);
		gGINFO.player = 2;
		this->_tanks = 4;
		this->cT[0].Init({ MAP_W / 2, MAP_H - 2 }, 100,
			1, 'D', 'A', 0x04);
		this->cT[0].Init({ MAP_W / 2, MAP_H + 4 }, 100,
			1, 'W', 'A', 0x09);
		//this->cT[1].Init({ 1, 1 }, 0, 2, 'D', 'A', 0x09);
		this->cT[2].Init({ MAP_W / 2 - 6, 1 }, 100,
			3, 'S', 'B', deCol敌方速度型, 100);
		this->cT[3].Init({ MAP_W / 2 + 6, 1 }, 100,
			4, 'S', 'B', deCol敌方超重型, 300, 51);
		tmap.TANKER = 1;
		PM->SetMap(cT[tmap.TANKER - 1].GetOldXY(), tmap, true);
	}
	gGINFO.start = GetTickCount64();	//存储当前时间
#pragma endregion

//开始循环游戏
#pragma region 开始循环游戏
	PV->PrintMap();
	PV->PrintMap(cT[0]);
	do {
		msecond = (unsigned int)(
			GetTickCount64() - gGINFO.start);
		PrintGInfo(msecond);
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
		xy = cT[0].GetOldXY();
		this->CheckTank(xy, 1);
		if (gGINFO.player == 2) {
			xy = cT[1].GetOldXY();
			this->CheckTank(xy, 2);
		}
		for (i = 0; i < gGINFO.player; i++)
		{
			xy = cT[i].GetOldXY();
			this->CheckTank(xy, i + 1);
			if (!cT[i].IsAlive()) {
				bGame = NULL;
				break;
			}
		}
#pragma endregion
		Sleep(100);
	} while (bGame);
	msecond = (unsigned int)(
		GetTickCount64() - gGINFO.start);
	gGINFO.count = 8;
	this->PrintGInfo(msecond, 0x0C);
	PV->PrintPoint({ MAP_W / 2 - 6,MAP_H / 2 },
		"游戏结束！按ESC退出！", 0x0C);
	printf_s("玩家%d死亡！", i + 1);
	while (!KEY_DOWN(KEY_ESC))
		Sleep(55);
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
		else if(tmap.Bullet == true){
			return	false;
		}
		else {
			map[xy.Y][xy.X].Bullet = true;//设置地图
			PV->PrintPoint(xy, deStr子弹);//画子弹
		}
	}
	else return false;		//如果试图移动失败，则返回假
	return true;
}

bool CCtrl::MoveTank()
{
	CHARMAP tmap = { 0 };
	COORD xy = { 0,0 };
	byte dir, i;
	for (i = 2; i < this->_tanks; i++) {
		if (!cT[i].IsAlive()) continue;
		tmap.TANKER = i + 1;
		do {
			dir = NULL;
			gAPI.RandDir(dir);
		} while (!cT[i].TryMove(dir));
		xy = cT[i].Move();
		if (gAPI.GetRand() % 10 < 2) {	//五分之一概率开炮
			this->AddBullet(i + 1);
		}
		if (!CheckTank(xy, i + 1)) {	//返回假时,处理重置坦克
			CHARMAP tmap = {0};
			PM->SetMap(xy, tmap, true);
			if (i == 2)
				this->cT[2].Init({ MAP_W / 2 - 6, 1 },
					100, 3, 'S', 'B', deCol敌方速度型);
		}
		PM->SetMap(cT[i].GetOldXY(), tmap, true);
		PV->PrintMap(cT[i]);
	}
	return false;
}

bool CCtrl::CheckTank(COORD& xy, byte tid)
{
	CHARMAP tmap;
	SHORT x, y; int power;
	byte bid = 0;
	for (x = -1; x < 2; x++)
	{
		for (y = -1; y < 2; y++)
		{
			tmap = map[xy.Y + y][xy.X + x];
			if (tmap.Bullet == true) {
				FindBullet({ xy.X + x,xy.Y + y }, &bid);
				if (tid < 3) {		//重画控制坦克
					tmap.TANKER = tid;
					PM->SetMap(xy, tmap, true);
					PV->PrintMap(cT[tid - 1]);
				}
				if (cT[tid - 1].GetTeam() ==
					cT[bid - 1].GetTeam())
					power = cT[tid - 1].GetBlood();
				else
					power = cT[tid - 1].GetBlood() -
					cT[bid - 1].GetPower();
				if (power > 0)
					cT[tid - 1].SetBlood(power);
				else {
					cT[tid - 1].SetBlood(0);
					cT[bid - 1].AddKills();
				}
				return power > 0;
			}
		}
	}
	return true;
}

void CCtrl::PrintGInfo(unsigned int& msecond, WORD color)
{
	//	https://blog.csdn.net/xingcen/article/details/55669054
	if (++gGINFO.count % 10 < 5) return;//0.5秒刷新界面
	else gGINFO.count = 0x00;			//重置时间
	char tick[MAX_PATH];				//用于存储格式的时间
	time(&gGINFO.now);					//获取系统日期和时间
	struct tm t;						//tm结构指针
	localtime_s(&t, &gGINFO.now);		//获取当地日期和时间
	COORD xy = { MAP_W + 2,0 };			//初始化打印坐标
	strftime(tick, MAX_PATH, "当前时间：%Y年%m月%d日%H:%M:%S", &t);
	PV->PrintPoint(xy, tick, color);	//打印格式化的时间
	xy.Y += 2;
	unsigned short second = msecond / 1000;
	sprintf_s(tick, "游戏时间：%02d:%02d:%03d",
		second / 60, second % 60, msecond % 1000);
	PV->PrintPoint(xy, tick, color);
	xy.Y += 4;
	byte i;
	for (i = 0; i < 5; i++, xy.Y++)
		PV->PrintPoint(xy, GAMEDesc[i], color);
	xy.Y++; int itmp[5] = { 0 }, itmp2[5] = { 0 };
	cT[0].GetInfo(itmp);
	if (gGINFO.player == 2)
		cT[2].GetInfo(itmp2);
	for (i = 0; i < 5; i++, ++xy.Y)
	{
		if (gGINFO.player == 1)
			sprintf_s(tick, PlayerInfo[i], itmp[i]);
		else
			sprintf_s(tick, PlayerInfo2[i], itmp[i], itmp2[i]);
		PV->PrintPoint(xy, tick, color);
	}
}

bool CCtrl::FindBullet(COORD xy, byte* tid)
{
	COORD bxy = { 0,0 };
	auto begin = _bullets.begin();
	while (begin != _bullets.end()) {
		bxy = (*begin).GetOldXY();
		if (xy.X == bxy.X && xy.Y == bxy.Y) {
			*tid = (*begin).GetTid();
			map[xy.Y][xy.X].Bullet = false;
			PV->PrintPoint(xy, deStr空地);
			begin = _bullets.erase(begin);
			return true;
		}
		++begin;
	}
	++begin;		//不可能情况
	return false;
}
