#include "CMap.h"

/*按菜单初始化地图*/
bool CMap::SetMap(int index)
{
	short x = 3, y = 5, y2 = 5, x2;
	if (index == 1) {			//关卡1
#pragma region 注册河流			//注册河流
		for (x2 = MAP_W / 3 - 3, y = 0; y < 2; y++)
			for (x = 0; x < 4; x++)
				map[y + y2][x + x2].Rever = true;
		for (x2 += 6, y = 0; y < 2; y++)
			for (x = 0; x < 4; x++)
				map[y + y2][x + x2].Rever = true;
		for (x2 += 6, y = 0; y < 2; y++)
			for (x = 0; x < 4; x++)
				map[y + y2][x + x2].Rever = true;
#pragma endregion
#pragma region 注册铁墙			//注册铁墙
		y2 += 5; x2 = MAP_W / 3 - 3;
		for (y = 0; y < 2; y++)
			for (x = 0; x < 3; x++)
				map[y + y2][x + x2].Wall = true;
		for (x2 += 6, y = 0; y < 2; y++)
			for (x = 0; x < 3; x++)
				map[y + y2][x + x2].Wall = true;
		for (x2 += 6, y = 0; y < 2; y++)
			for (x = 0; x < 3; x++)
				map[y + y2][x + x2].Wall = true;
#pragma endregion
#pragma region 注册草坪			//注册草坪
		y2 += 4; x2 = MAP_W / 3 - 3;
		for (y = 0; y < 3; y++)
			for (x = 0; x < 3; x++)
				map[y + y2][x + x2].Grass = true;
		for (x2 += 6, y = 0; y < 3; y++)
			for (x = 0; x < 3; x++)
				map[y + y2][x + x2].Grass = true;
		for (x2 += 6, y = 0; y < 3; y++)
			for (x = 0; x < 3; x++)
				map[y + y2][x + x2].Grass = true;
#pragma endregion
#pragma region 注册土墙			//注册土墙
		y2 += 5; x2 = MAP_W / 3 - 3;
		for (y = 0; y < 3; y++)
			for (x = 0; x < 6; x++)
				map[y + y2][x + x2].Earth = true;
		for (x2 += 6, y = 0; y < 3; y++)
			for (x = 0; x < 6; x++)
				map[y + y2][x + x2].Earth = true;
		for (x2 += 6, y = 0; y < 3; y++)
			for (x = 0; x < 3; x++)
				map[y + y2][x + x2].Earth = true;
#pragma endregion
		return true;
	}
	else if (index == 2) {		//关卡2
#pragma region 注册土墙1		//注册土墙1
		for (x2 = 0, y = 0; y < 3; y++)
			for (x = 0; x < MAP_W; x++)
				map[y + y2][x + x2].Earth = true;
#pragma endregion
#pragma region 注册铁墙			//注册铁墙
		y2 += 5; x2 = MAP_W / 3 - 4;
		for (y = 0; y < 2; y++)
			for (x = 0; x < 6; x++)
				map[y + y2][x + x2].Rever = true;
		for (x2 += 6, y = 0; y < 2; y++)
			for (x = 0; x < 6; x++)
				map[y + y2][x + x2].Rever = true;
		for (x2 += 6, y = 0; y < 2; y++)
			for (x = 0; x < 6; x++)
				map[y + y2][x + x2].Rever = true;
#pragma endregion
#pragma region 注册草坪			//注册草坪
		y2 += 4; x2 = 0;
		for (y = 0; y < 3; y++)
			for (x = 0; x < MAP_W; x++)
				map[y + y2][x + x2].Grass = true;
#pragma endregion
#pragma region 注册土墙2		//注册土墙2
		y2 += 5; x2 = 0;
		for (y = 0; y < 3; y++)
			for (x = 0; x < MAP_W; x++)
				map[y + y2][x + x2].Earth = true;
#pragma endregion
		return true;
	}
	else {
		gGINFO.levels = 1;
	}
	return false;
}

/*按坐标信息改变地图*/
bool CMap::SetMap(COORD xy, CHARMAP stmap, bool isTank)
{
	short i, j;
	if (isTank) {
		for (i = -1; i < 2; i++)
		{
			for (j = -1; j < 2; j++)
			{
				map[xy.Y + j][xy.X + i].TANKER = stmap.TANKER;
			}
		}
	}
	else {
		map[xy.Y][xy.X] = stmap;
	}
	return true;
}

