#pragma once
#include <list>
using std::list;
#include "CView.h"
#include "CMap.h"
#include "CAStar.h"

class CCtrl
{
public:
	CCtrl(COORD xy);
	~CCtrl();

	/*初始化窗口*/
	bool InitCMD();

	/*开始游戏函数*/
	int Go(int GameMode = 1, int Checkpoint = 1);

	/* 判断按键函数
	用于按键判断	*/
	void GoKey(byte& key, unsigned int msecond);



private:	//	//	//对内函数
	/*	初始化坦克函数
	用于初始化坦克	*/
	void SetTank(int TankI);

	/*	新增子弹函数
	用于检测子弹是否能够生成	*/
	bool AddBullet(byte tid);

	/*	子弹移动函数
	用于移动子弹	*/
	bool MoveBullet(CBullet& att, bool clean = false);

	/*	移动坦克函数
	用于移动循环AI坦克	*/
	bool MoveTank(unsigned int& mSecond);

	/*	检查坦克碰撞函数
	用于检查坦克是否碰撞	*/
	bool CheckTank(COORD& xy, byte tid);

	/*	游戏信息打印函数
	用于地图右侧的游戏信息绘制	*/
	void PrintGInfo(unsigned int& msecond, WORD color = 0x0E);

	/*	找子弹函数
	通过坐标返回子弹的归属坦克	*/
	bool FindBullet(COORD xy, byte* tid);

	/*	自定义地图函数
	用于窗口画地图	*/
	bool DrawMap();

private:	//	//	//对内变量
	CTanker cT[7];	//坦克数组
	CView*	PV;		//视图函数
	CMap*	PM;		//地图函数
	CAStar	PA;		//寻路算法
	COORD	_maxXY;	//内部窗口大小
	byte	_tanks = 0x04;	//坦克数量
	list<CBullet> _bullets;	//子弹数组
};
