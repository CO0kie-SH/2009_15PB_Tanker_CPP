#pragma once
#include <Windows.h>

class CData
{
public:
	void API_OutputDebugPrintf(const char* strOutputString, ...);
	CData();
	~CData();

private:

};

#pragma region 注册宏定义

//定义地图长宽
#define MAP_W 30
#define MAP_H 27
#define TANKER_MAX 15

//定义热键
#define KEY_ESC 27

#pragma endregion

#pragma region 字符串索引

#pragma endregion


#pragma region 定义结构体

//定义地图
typedef struct _CHARMAP {
	unsigned char TANKER : 4;
	unsigned char WALL : 1;
	unsigned char BLOCK : 1;
	unsigned char BULLET : 1;
}CHARMAP;

//定义游戏世界参数

typedef struct _GAMEINFO {
	byte menu = 0x01;
	byte statu = 0x00;
	time_t now;								//声明time_t类型变量
	ULONGLONG start;
}GAMEINFO, * PGAMEINFO;


#pragma endregion


#pragma region 全局变量
extern CHARMAP map[MAP_H][MAP_W];			//全局地图
extern HANDLE gOUTPUT;						//窗口输出句柄
extern PGAMEINFO gGINFO;
extern CData gAPI;
#pragma endregion