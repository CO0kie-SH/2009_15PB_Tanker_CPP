#pragma once

#include "CTanker.h"
#include "CBullet.h"
#include <iostream>
using std::cout;
using std::endl;
using std::flush;
#include "CBgm.h"
class CView
{
public:
	CBgm	PB;	//定义BGM

	bool InitWindow(COORD& xy, bool isCursor);	//初始化窗口
	void PrintPoint(COORD xy, const char* text, WORD color = 0x00);
	void PrintMap();							//刷新窗口、绘画地图
	void PrintMap(CHARMAP tmap);				//重载绘画地图点
	void PrintMap(CTanker& that, bool clean = false);	//重载绘画坦克
	void PrintMap(byte menuIndex, bool err = false);	//重载绘制游戏信息
	bool SaveGame(CTanker* Tanks);				//保存游戏
	bool ReadGame(CTanker* Tanks);				//读取游戏
private:

};