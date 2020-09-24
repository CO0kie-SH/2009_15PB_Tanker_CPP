#pragma once
#include "CTanker.h"
class CMap
{
public:
	/*按菜单初始化地图*/
	bool SetMap(int index = 1);

	/*按坐标信息改变地图*/
	bool SetMap(COORD xy, CHARMAP stmap, bool isTank = false);
};