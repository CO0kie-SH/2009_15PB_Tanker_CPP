#include "CAStar.h"
#include <algorithm>


void CAStar::Go()
{
	memcpy_s(tmap, MAP_LEN, map, MAP_LEN);
	_open.push_back({ _hxy.X,_hxy.Y });
	//Print(_exy, "尾");
	if (FindOpen() && GetPath()) {
		//找到路径
		_find = (*(_path.end() - 1)).XY;
		//Print(_find, "下", 0x0A);
	}
	_open.clear(); _close.clear();
}

bool CAStar::FindOpen()
{
	COORDEX OldPos;
	COORDEX newPos;
	vector<COORD> points;
	char buff[250];
	while (!_open.empty())
	{
		auto min = min_element(_open.rbegin(), _open.rend(), compare);
		sprintf_s(buff, "%2d", (*min).g_value);
		Print((*min).XY, buff);
		Sleep(5);
		OldPos = (*min);					//获取点
		this->tmap[OldPos.XY.Y][OldPos.XY.X].Wall = true;
		_close.push_back(OldPos);			//加入已找的表
		_open.erase(--min.base());			//删除待找的表
		points.clear();						//初始化回调数组
		if (!KuoSan(OldPos.XY, points))		//取扩散数组，如果
			continue;						//没有，则到循环尾

		for (auto& point : points) {		//循环扩散点
			newPos = COORDEX{ point.X,point.Y };
			newPos.Init(OldPos, _exy);		//初始化坐标
			if (newPos == _exy) {			//如果坐标为终点
				_find = point;				//倒序赋值
				_close.push_back(newPos);	//加入已表
				return true;				//跳出循环
			}
			Print(point, "待");
			_open.push_back(newPos);
		}
	}
	return false;
}

bool CAStar::KuoSan(COORD oXY, vector<COORD>& points)
{
	COORD offset[4] = { {-1, 0}, {1, 0}, {0,-1}, {0, 1} };
	COORD addPos;
	for (int i = 0; i < 4; ++i)
	{
		addPos = {
			offset[i].X + oXY.X,
			offset[i].Y + oXY.Y
		};
		//gAPI.OutputDebugPrintf("%d,P=%p == %d,%d,%d\n",
		//	i, &oXY, i, addPos.X, addPos.Y);
		if (Check2(addPos))
			points.push_back(addPos);
	}
	return !points.empty();
}

bool CAStar::Check2(COORD& tryXY)
{
	if (tryXY.X < 0 || tryXY.Y < 0)
		return false;
	else if (tryXY.X >= MAP_W || tryXY.Y >= MAP_H)
		return false;
	else {
		CHARMAP tmp = tmap[tryXY.Y][tryXY.X];
		if (tmp.TANKER || tmp.Wall)			//铁墙、坦克不能穿过
			return false;					//标识返回值
	}
	tmap[tryXY.Y][tryXY.X].Wall = true;
	return true;
}

bool CAStar::GetPath()
{
	int i;
	for (i = _close.size() - 1; i >= 0; --i)
	{
		if (_close[i] == _hxy) break;
		if (_close[i] == _find) {
			Print(_find, "路");
			_path.push_back(_close[i]);
			_find = _close[i].oXY;
		}
	}
	return _close[i] == _hxy;
}

/*	绘制地图
	用于绘制地图	*/
void CAStar::Print(COORD& xy, const char* text, WORD color)
{
	COORD pxy = { xy.X,xy.Y };
	if (color != 0x00)							//如果传入颜色
		SetConsoleTextAttribute(gOUTPUT, color);//则改变画刷
	pxy.X = pxy.X * 2 + 2; pxy.Y++;				//转换窗口坐标
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pxy);
 	printf_s("%s", text);
	if (color != 0x00)
		SetConsoleTextAttribute(gOUTPUT, 0x0F);	//还原画刷
}

/*	初始化坐标函数
	用于计算坐标的点fgh值	*/
void COORDEX::Init(const COORDEX& oXY, COORD& END)
{
	this->oXY = oXY.XY;
	this->g_value = oXY.g_value + 1;
	this->h_value = abs(XY.X - END.X) + abs(XY.Y - END.Y);
	this->f_value = this->g_value + this->h_value;
}
