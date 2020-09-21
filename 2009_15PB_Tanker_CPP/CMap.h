#pragma once
#include "CTanker.h"
class CMap
{
public:
	CMap();
	~CMap();
	bool SetMap(int index = 1);
	bool SetMap(COORD xy, CHARMAP stmap, bool isTank = false);
private:
};