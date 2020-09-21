#pragma once
#include <Windows.h>

class CData
{
public:
	void OutputDebugPrintf(const char* strOutputString, ...);
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
const char INFOMenu[][31] = {
	{"欢迎使用CO0Kie丶初代坦克大战。"},
	{"①单人游戏"},
	{"②双人游戏"},
	{"③读取游戏"},
	{"④编辑地图"},
	{"⑤ 退 出"}
};
const char GAMEDesc[][20] = {
	{"玩家\t玩家 1\t玩家 2"},
	{"向上\t  W\t  I"},
	{"向左\t  A\t  J"},
	{"向下\t  S\t  K"},
	{"向右\t  D\t  L"},
	{"开火\t  F\t  ;"}
};
const char PlayerInfo[][15] = {
	{"血量\t  %d\t%d"},
	{"速度\t  %d\t%d"},
	{"攻击力  %d\t%d"},
	{"杀敌数  %d\t%d"},
	{"得  分  %d\t%d"}
};
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
#define 游戏 0x00
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