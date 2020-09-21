#pragma once

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
	CView*	PV;
	CMap*	PM;
	COORD	_maxXY;
	byte	_tanks = 0x04;
};
