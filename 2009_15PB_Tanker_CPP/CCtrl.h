#pragma once
#include <list>
using std::list;
#include "CView.h"
#include "CMap.h"

class CCtrl
{
public:
	CCtrl(COORD xy);
	~CCtrl();

	bool InitCMD();
	int Go(int index = 1);
	
	CTanker cT[8];
private:
	bool AddBullet(byte tid);
	bool MoveBullet(CBullet& att, bool clean = false);
	bool MoveTank();
	bool CheckTank(COORD& xy, byte tid);
	void PrintGInfo(unsigned int& msecond, WORD color = 0x0E);
	bool FindBullet(COORD xy, byte* tid);
private:
	CView*	PV;
	CMap*	PM;
	COORD	_maxXY;
	byte	_tanks = 0x04;
	list<CBullet> _bullets;
};
