#pragma once

#include "CView.h"
#include "CMap.h"

class CCtrl
{
public:
	CCtrl(COORD xy);
	~CCtrl();

	bool InitCMD();

	CTanker cT[3];
private:
	CView* PV;
	CMap* PM;
	COORD _maxXY;
};
