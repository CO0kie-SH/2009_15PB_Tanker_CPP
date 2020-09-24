#pragma once
#include "CData.h"
#include <vector>
using std::vector;


class COORDEX
{
public:
	COORD XY = { 0 };	// 保存子节点的坐标
	COORD oXY = { 0 };	// 保存父节点的坐标
	int h_value = 0;		// 当前点到终点的最优路线步数
	int g_value = 0;		// 从起点到当前点走过的步数
	int f_value = 0;		// f = g + h; 理论步数
	COORD GetPos() { return XY; };
	void Init(const COORDEX& oXY, COORD& END);

	// 比较函数，用于比较两个节点值是否相等
	bool operator==(const COORD& pos)
	{
		return XY.X == pos.X && XY.Y == pos.Y;
	}
};

// 要求所有作为回调函数(作为参数进行传递的函数)的参数都是静态函数
static bool compare(const COORDEX& point1, const COORDEX& point2)
{
	return point1.f_value < point2.f_value;
}

class CAStar
{
public:
	void Go();
	bool FindOpen();
	bool KuoSan(COORD oXY, vector<COORD>& points);
	bool Check2(COORD& tryXY);
	bool GetPath();
private:
	void Print(COORD& xy, const char* text, WORD color = 0x00);
	CHARMAP tmap[MAP_H][MAP_W];
	COORD _hxy = { 1,1 };
	COORD _exy = { 20,9 };
	COORD _find;
	// 保存等待查找的所有位置
	vector<COORDEX> _open;

	// 保存已经查找的所有位置
	vector<COORDEX> _close;

	// 保存最短路径
	vector<COORDEX> _path;
};

