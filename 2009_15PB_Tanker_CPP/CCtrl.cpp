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
	gOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);//得到输出句柄
	gAPI.OutputDebugPrintf("初始化。");
}

CCtrl::~CCtrl()
{
	delete PV;				//释放视图类&地图类
	delete PM;
	//CloseHandle(gOUTPUT);	//释放输出句柄
}

bool CCtrl::InitCMD()
{
	//初始化窗口光标、大小
	if (PV->InitWindow(_maxXY, false) == false)
		return false;
	byte menu = 1;
	PV->PrintMap(); PV->PrintMap(menu);
	do
	{
		PV->PrintMap(menu);
		switch (_getch()) {
		case 'W': case 'I': case 'w': case 'i':
			if (--menu < 1) menu = 5; break;
		case 'S': case 'K': case 's': case 'k':
			if (++menu > 5) menu = 1; break;
		case KEY_ESC:
			if (menu == 5) return false;
			menu = 5; break;
		case 32: {
			if (menu == 5) return false;
			else if (menu >= 1 && menu <= 2)
				while (Go(menu, gGINFO.levels)) {
					++gGINFO.levels;
				}
			else if (4 == menu)
				Go(de存档游戏);
			else {
				if (this->DrawMap())
					Go(de自定义地图);
				PV->PrintMap();
				gGINFO.menu = 0x00;
			}
			break; }
		default: break;
		}
	} while (true);
	return false;
}
#pragma endregion

//游戏开始
#pragma region 初始化游戏世界
//用于重置坦克
void CCtrl::SetTank(int TankI)
{
	CHARMAP tmap = { 0 };
	if (2 == TankI)
		this->cT[2].Init({ MAP_W / 2 - 6, 1 }, 50,
			3, 'S', 'B', deCol敌方速度型, 100);
	else if (3 == TankI)
		this->cT[3].Init({ MAP_W / 2 + 6, 1 }, 60,
			4, 'S', 'B', deCol敌方超重型, 500, 44);
	else if (0 == TankI) {
		this->cT[0].Init({ MAP_W / 2, MAP_H - 2 }, 100,
			1, 'D', 'A', 0x04);
		tmap.TANKER = TankI + 1;
		PM->SetMap(cT[TankI].GetOldXY(), tmap, true);
		PV->PrintMap(cT[TankI]);
	}
	else if (1 == TankI) {
		this->cT[1].Init({ MAP_W / 2 + 4, MAP_H - 2 }, 100,
			2, 'W', 'A', 0x09);
		tmap.TANKER = TankI + 1;
		PM->SetMap(cT[TankI].GetOldXY(), tmap, true);
		PV->PrintMap(cT[TankI]);
	}
}

int CCtrl::Go(int GameMode, int Checkpoint)
{
	byte bGame = 'G', key = 0, i = 0;
	COORD xy = { 0,0 }; CHARMAP tmap = { 0 };
	unsigned int msecond = 0; int nums = 0;
	this->_tanks = 4;
	if (de单人游戏闯关 == GameMode) {		//初始化单人游戏
		if (!PM->SetMap(Checkpoint)) return 0;
		PV->PrintMap();
		gGINFO.player = 1; this->SetTank(0);
		this->cT[1].Init({ 1, 1 }, 0, 2);
	}
	else if (de双人游戏闯关 == GameMode) {	//初始化双人游戏
		if (!PM->SetMap(Checkpoint)) return 0;
		PV->PrintMap(); gGINFO.player = 2;
		this->SetTank(0); this->SetTank(1);
	}
	else if (de自定义地图 == GameMode) {	//初始化自定义游戏
		gGINFO.player = 1; this->SetTank(0);
		this->cT[1].Init({ 1, 1 }, 0, 2);
	}
	this->SetTank(2); this->SetTank(3);
	if (de存档游戏 == GameMode) {		//初始化存档
		this->PV->ReadGame(cT);
		this->PV->PrintMap();
		gGINFO.player = cT[1].IsAlive() ? 2 : 1;
		tmap = { 0 };
		for (i = 0; i < _tanks; i++)
		{
			PM->SetMap(cT[i].GetOldXY(), tmap, true);
			cT[i].mSecond = 0;
			this->PV->PrintMap(cT[i]);
		}
		Checkpoint = gGINFO.levels;
	}
	gGINFO.count = 7;
#pragma endregion

//开始循环游戏
#pragma region 开始循环游戏
	do {
		msecond = (unsigned int)(
			GetTickCount64() - gGINFO.start);
		PrintGInfo(msecond);
#pragma region AI控制逻辑段
		tmap.TANKER = 0;						//设置地图坦克false
		for (i = 2; i < this->_tanks; i++) {	//循环清零
			PM->SetMap(cT[i].GetOldXY(), tmap, true);
			PV->PrintMap(cT[i], true);			//隐藏坦克
		}
		auto begin = _bullets.begin();			//子弹流程
		while (begin != _bullets.end()) {		//循环子弹
			if (MoveBullet(*begin, true)) ++begin;	//移动
			else begin = _bullets.erase(begin);	//如果失败则删除
		}
		MoveTank(msecond);						//调用移动坦克函数
		begin = _bullets.begin();				//循环开始
		while (begin != _bullets.end()) {
			if ((*begin).IsAlive()) ++begin;
			else begin = _bullets.erase(begin);
		}
#pragma endregion
#pragma region 人工控制逻辑段
		this->GoKey(key, msecond);
		if (key == KEY_ESC)	bGame = NULL;
		for (i = 0,nums=0; i < gGINFO.player; i++)
		{
			xy = cT[i].GetOldXY();
			nums += cT[i].GetInfo2();
			this->CheckTank(xy, i + 1);
			if (!cT[i].IsAlive()) {
				bGame = NULL; break;
			}
		}
#pragma endregion
#pragma region 结尾处判断
		if (de存档游戏 != GameMode && nums > 99) bGame = NULL;
		if (key == 'P') {
			gGINFO.start = (unsigned int)(
				GetTickCount64() - gGINFO.start);		//存储游戏用时
			msecond = 0x02; gGINFO.count = 7;			//标识游戏暂停
			this->PrintGInfo(msecond, deCol土墙);		//打印操作方法
			while (i = _getch()) {
				if (i == 'N' || i == 'n') {
					gGINFO.start = (unsigned int)
						(GetTickCount64() - gGINFO.start);	//转换时间
					break;
				}
				else if (i == 'Y' || i == 'y') {
					gGINFO.start = (unsigned int)
						(GetTickCount64() - gGINFO.start);	//转换时间
					this->PV->SaveGame(cT);
					break;
				}
			}
			msecond = 0x03; gGINFO.count = 7;			//标识游戏暂停
			this->PrintGInfo(msecond);			//打印操作方法
		}
		if (bGame) Sleep(78);
#pragma endregion
	} while (bGame);
#pragma region 单局游戏结算
	gGINFO.count = 7; PrintGInfo(msecond, 0x0C);
	PV->PrintPoint({ MAP_W / 2 - 6,MAP_H / 2 },
		"游戏结束！按Y退出！", 0x0C);
	if (nums > 99 && de存档游戏 != GameMode)
		printf_s("过关啦！下一关%d", Checkpoint + 1);
	else if (key != KEY_ESC) {
		for (i = 0; i < gGINFO.player; i++)
			if (!cT[i].IsAlive())
				printf_s("玩家%d死亡！", i + 1);
	}
	while (true) {
		if ((i = _getch()) && KEY_DOWN('Y'))
			if (i == 'Y' || i == 'y')
				break;
		Sleep(11);
	}
	memset(map, 0, MAP_H * MAP_W);			//单局游戏结束初始化地图
	PV->PrintMap(); gGINFO.menu = 0x00;		//初始化界面
	this->_bullets.clear();					//清除子弹数组
	if (nums > 99) return 1;
#pragma endregion
	return 0;
}

void CCtrl::GoKey(byte& key, unsigned int msecond)
{
	CHARMAP tmap = { 0 };					//初始化地图参数
	switch (key = gAPI.GetKey()) {			//通过API返回键值
	case 'W':case 'A':case 'S':case 'D':	//方向键移动坦克1
		//如果	坦克速度>(游戏时间-坦克上次时间)则打印返回
		if (cT[0].GetSpeed() > (int)(msecond - cT[0].mSecond)) {
			break;
		}	cT[0].mSecond = msecond;
		tmap.TANKER = 0;					//标识地图为坦克NULL
		PM->SetMap(cT[0].GetOldXY(), tmap, true);
		if (cT[0].TryMove(key) == true) {	//成功移动
			PV->PrintMap(cT[0], true);		//清除旧的坦克
			cT[0].Move();					//赋值新的坐标
			tmap.TANKER = 1;				//标识地图为玩家1
			PM->SetMap(cT[0].GetOldXY(), tmap, true);
			PV->PrintMap(cT[0]);			//打印新的坦克
		}
		else {
			tmap.TANKER = 1;				//标识地图为坦克1
			PM->SetMap(cT[0].GetOldXY(), tmap, true);
		} break;
	case 'F': this->AddBullet(0x01); break;
	case 'H': this->AddBullet(0x02); break;
	case 'I':case 'J':case 'K':case 'L':	//方向键移动坦克2
		//如果	坦克速度>(游戏时间-坦克上次时间)则打印返回
		if (cT[1].GetSpeed() > (int)(msecond - cT[1].mSecond)) {
			break;
		}	cT[1].mSecond = msecond;
		tmap.TANKER = 0;					//标识地图为坦克NULL
		PM->SetMap(cT[1].GetOldXY(), tmap, true);
		if (cT[1].TryMove(key) == true) {	//成功移动
			PV->PrintMap(cT[1], true);		//清除旧的坦克
			cT[1].Move();					//赋值新的坐标
			tmap.TANKER = 2;				//标识地图为玩家2
			PM->SetMap(cT[1].GetOldXY(), tmap, true);
			PV->PrintMap(cT[1]);			//打印新的坦克
		}
		else {
			tmap.TANKER = 2;				//标识地图为坦克2
			PM->SetMap(cT[2].GetOldXY(), tmap, true);
		} break;
	//case KEY_ESC: bGame = 0x00; break;
	default: break;
	}
}

#pragma endregion


//坦克、子弹功能区
#pragma region 坦克子弹功能区

/*	新增子弹函数
	用于检测子弹是否能够生成	*/
bool CCtrl::AddBullet(byte tid)
{
	CTanker& tank = this->cT[tid-1];		//传入坦克
	CBullet att;							//定义子弹
	COORD xy = tank.GetOldXY();				//获取坦克坐标
	if (map[xy.Y][xy.X].Bullet == true)		//如果坐标为子弹，则返回
		return false;						//函数返回
	if(att.Init(xy, tank.GetDir(false),tank.GetTid())){
		if (MoveBullet(att)) {				//初始化子弹后，尝试移动
			_bullets.push_back(att);		//如果成功，则加入变量
			return true;					//函数返回
		}
	}
	return false;
}

/*	子弹移动函数
	用于移动子弹	*/
bool CCtrl::MoveBullet(CBullet& att, bool clean)
{
	if (!att.IsAlive()) return false;			//如果本身死亡，则返回
	COORD xy = att.GetOldXY();					//获取子弹坐标
	CHARMAP tmap = map[xy.Y][xy.X];				//获取坐标地图点
	if (clean) {								//判断重画条件
		map[xy.Y][xy.X].Bullet = false;			//如果传入还原子弹，则重画
		if (tmap.Rever == true)					//还原蓝色河流
			PV->PrintPoint(xy, deStr河流, deCol河流);
		else if (tmap.Grass == true)			//还原绿色草坪
			PV->PrintPoint(xy, deStr草坪, deCol草坪);
		else PV->PrintPoint(xy, deStr空地);		//还原空地
	}
	if (att.TryMove()) {						//试图移动子弹
		xy = att.Move();						//如果成功，则移动
		tmap = map[xy.Y][xy.X];					//并刷新地图
		if (tmap.Rever == true)					//河流则画蓝色的子弹
			PV->PrintPoint(xy, deStr子弹, deCol河流);
		else if (tmap.Grass == true) {}			//草坪则不重画
		else if (tmap.Earth == true) {			//遇到土墙则销毁
			att.SetBlood(0);					//销毁子弹
			tmap = { 0 };
			PM->SetMap(xy, tmap);				//重置地图
			PV->PrintPoint(xy, deStr空地);		//画空地
			return false;						//函数返回
		}
		else if(tmap.Bullet == true){			//子弹与子弹碰撞
			FindBullet(xy, nullptr);			//找到子弹点并销毁
			att.SetBlood(0);					//销毁自身
			map[xy.Y][xy.X].Bullet = false;		//设置地图
			PV->PrintPoint(xy, deStr空地);		//画空地
			return	false;						//函数返回
		} else {								//否则
			map[xy.Y][xy.X].Bullet = true;		//设置地图
			PV->PrintPoint(xy, deStr子弹);		//画子弹
		}
	}
	else return false;		//如果试图移动失败，则返回假
	return true;
}

/*	移动坦克函数
	用于移动循环AI坦克	*/
bool CCtrl::MoveTank(unsigned int& mSecond)
{
	CHARMAP tmap = { 0 };						//定义变量
	COORD xy = { 0,0 };							//定义变量
	byte dir, i, isMove = NULL;					//定义变量
	for (i = 2; i < this->_tanks; i++) {		//循环坦克
		if (!cT[i].IsAlive()) continue;			//如果死亡，则跳出
		tmap.TANKER = i + 1;					//将地图设置为坦克
		xy = cT[i].GetOldXY();					//获取坦克坐标

		//如果	坦克速度<(已过去的时间) (游戏时间-坦克上次时间)则移动
		if (cT[i].GetSpeed() < (int)(mSecond - cT[i].mSecond)) {
			cT[i].mSecond = mSecond;			//重置坦克时间
			do {								//循环移动坦克
				gAPI.RandDir(dir);				//通过API（1/24）%
			} while (!cT[i].TryMove(dir));		//的概率设置方向
			xy = cT[i].Move();					//否则移动坦克
			if (gAPI.GetRand() % 10 < 2) {		//五分之一概率开炮
				this->AddBullet(i + 1);			//生成子弹
			}	isMove = true;					//设置移动成功
		}
		if (!CheckTank(xy, i + 1)) {			//返回假时,处理重置坦克
			tmap = { 0 };
			PM->SetMap(xy, tmap, true);			//设置地图
			this->SetTank(i);					//重置
		}
		PM->SetMap(cT[i].GetOldXY(), tmap, true);	//设置地图
		PV->PrintMap(cT[i]);						//打印坦克
	}
	return isMove;
}

/*	检查坦克碰撞函数
	用于检查坦克是否碰撞	*/
bool CCtrl::CheckTank(COORD& xy, byte tid)
{
	CHARMAP tmap;								//定义变量
	SHORT x, y; int power;						//定义变量
	byte bid = 0;								//定义变量
	for (x = -1; x < 2; x++) {					//循环坦克体x
		for (y = -1; y < 2; y++) {				//循环坦克体y
			tmap = map[xy.Y + y][xy.X + x];		//获取该点坐标
			if (tmap.Bullet == true) {			//如果该点为子弹
				FindBullet({ xy.X + x,xy.Y + y }, &bid);//函数找出坦克
				if (tid < 3) {					//如果传入玩家坦克
					tmap.TANKER = tid;			//重新绘制
					PM->SetMap(xy, tmap, true);	//设置地图
					PV->PrintMap(cT[tid - 1]);	//打印坦克
				}
				if (cT[tid - 1].GetTeam() ==	//获取子弹强度
					cT[bid - 1].GetTeam())		//获取子弹强度
					power = cT[tid - 1].GetBlood();//获取子弹强度
				else
					power = cT[tid - 1].GetBlood() -
					cT[bid - 1].GetPower();		//获取子弹强度
				if (power > 0)					//如果相减后血量>0
					cT[tid - 1].SetBlood(power);//设置血量
				else {							//否则
					cT[tid - 1].SetBlood(0);	//设置血量0
					cT[bid - 1].AddKills();		//增加杀敌数
					gGINFO.count = 7;
				}
				return power > 0;				//返回是否存活
			}
		}
	}
	return true;
}

/*	找子弹函数
	通过坐标返回子弹的归属坦克	*/
bool CCtrl::FindBullet(COORD xy, byte* tid)
{
	COORD bxy = { 0,0 };						//定义变量
	auto begin = _bullets.begin();				//定义变量
	while (begin != _bullets.end()) {			//循环子弹数组
		bxy = (*begin).GetOldXY();				//获取子弹坐标
		if (xy.X == bxy.X && xy.Y == bxy.Y) {	//判断坐标
			if (tid == nullptr) {				//从子弹碰撞传入
				(*begin).SetBlood(0);			//则销毁子弹
				return true;					//返回函数
			}
			*tid = (*begin).GetTid();			//设置子弹归属
			map[xy.Y][xy.X].Bullet = false;		//设置地图为空
			PV->PrintPoint(xy, deStr空地);		//打印空地
			(*begin).SetBlood(0);				//清除子弹
			return true;						//返回函数
		}
		++begin;								//循环自增
	}
	return false;
}

#pragma endregion

//地图、视图功能区
#pragma region 地图视图功能区

/*	游戏信息打印函数
	用于地图右侧的游戏信息绘制	*/
void CCtrl::PrintGInfo(unsigned int& msecond, WORD color)
{
	//参考 https://blog.csdn.net/xingcen/article/details/55669054
	if (++gGINFO.count % 10 < 7) return;	//0.5秒刷新界面
	else gGINFO.count = 0x00;				//重置时间
	char tick[MAX_PATH], i;					//用于存储格式的时间
	time(&gGINFO.now);						//获取系统日期和时间
	struct tm t;							//tm结构指针
	localtime_s(&t, &gGINFO.now);			//获取当地日期和时间
	COORD xy = { MAP_W + 2,0 };				//初始化打印坐标
	strftime(tick, MAX_PATH, "当前时间：%Y年%m月%d日%H:%M:%S", &t);
	PV->PrintPoint(xy, tick, color);		//打印格式化的时间
	xy.Y += 2;								//换行
	unsigned short second = msecond / 1000;		//记录当前秒
	sprintf_s(tick, "游戏时间：%02d:%02d:%03d",	//格式化字符串
		second / 60, second % 60, msecond % 1000);
	PV->PrintPoint(xy, tick, color);		//打印游戏时间
	xy.Y += 4;
	if (0x01 == msecond) {					//如果是自定义地图
		for (i = 0; i < 7; i++, ++xy.Y)		//循环输出操作模式
			PV->PrintPoint(xy, szKEYs[
				color == 0xFF ? 5 : i], MColor[i]);
		return;
	}
	else if (0x02 == msecond) {
		for (i = 0; i < 5; i++, ++xy.Y)		//循环输出操作模式
			PV->PrintPoint(xy, cstr暂停[i], color);
		return;
	}
	else if (0x03 == msecond) {
		for (i = 0; i < 6; i++, ++xy.Y)		//循环输出操作模式
			PV->PrintPoint(xy, cstr暂停[4], color);
		return;
	}
	for (i = 0; i < 5; i++, xy.Y++)			//循环操作提示
		PV->PrintPoint(xy, GAMEDesc[i], color);		//打印提示
	xy.Y++; int itmp[5] = { 0 }, itmp2[5] = { 0 };	//定义玩家属性
	cT[0].GetInfo(itmp);					//获取玩家1
	if (gGINFO.player == 2)					//判断玩家数量
		cT[1].GetInfo(itmp2);				//获取玩家2信息
	for (i = 0; i < 6; i++, ++xy.Y) {		//循环打印玩家属性
		if (gGINFO.player == 1)				//判断玩家1或玩家2
			sprintf_s(tick, PlayerInfo[i], itmp[i]);
		else
			sprintf_s(tick, PlayerInfo2[i], itmp[i], itmp2[i]);
		PV->PrintPoint(xy, tick, color);	//打印玩家信息
	}
}

/*	自定义地图函数
	用于窗口画地图	*/
bool CCtrl::DrawMap()
{
	PV->PrintMap();									//刷新界面
	INPUT_RECORD ir = {};							//定义变量
	DWORD num = 0;									//定义变量
	COORD pos;										//定义变量
	CHARMAP tmap;									//定义变量
	byte key;										//定义变量
	unsigned int msecond = 0x01;					//定义变量
	memset(map, 0, MAP_H * MAP_W);					//重置地图
	gGINFO.count = 7; PrintGInfo(msecond);			//格式化屏幕数据
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);	//获取控制台输入句柄
	SetConsoleMode(hInput,
		ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);	//设置控制台模式，接收鼠标事件
	while ((key = gAPI.GetKey()) != 'J') {			//获取按键
		if (key == 'H') break; Sleep(10);			//并添加适当延迟
		ReadConsoleInput(hInput, &ir, 1, &num);		//读取输入流
		if (ir.EventType == MOUSE_EVENT) {			//判断鼠标事件
			pos = ir.Event.MouseEvent.dwMousePosition;	//获取鼠标所在位置
			if (pos.X<2 || pos.Y>MAP_H - 3)	continue;	//判断边界
			pos.X = pos.X / 2 - 1; --pos.Y;			//转换地图坐标
			if (pos.X >= MAP_W) continue;			//判断超过边界
			else if (pos.Y < 3) continue;			//判断超过边界
			tmap = { 0 };							//初始化地图点
			if ('D' == key) tmap = { 0 };			//判断按键画空地
			else if ('W' == key) tmap.Wall = true;	//画铁墙
			else if ('F' == key) tmap.Earth = true;	//画土墙
			else if ('G' == key) tmap.Grass = true;	//画草坪
			else if ('R' == key) tmap.Rever = true;	//画河流
			else continue;							//如果没有按键，则跳出
			map[pos.Y][pos.X] = tmap;				//设置地图
			pos.X = pos.X * 2 + 2; pos.Y++;			//转换窗口坐标，并设置
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
			PV->PrintMap(tmap);						//打印地图点
		}
	}
	gGINFO.count = 7; PrintGInfo(msecond, 0xFF);	//重画窗口信息
	return 'J' == key;								//返回操作按键
}
#pragma endregion
