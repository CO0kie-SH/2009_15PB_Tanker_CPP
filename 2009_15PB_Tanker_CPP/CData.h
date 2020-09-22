#pragma once
#include <ctime>
#include <Windows.h>
class CData
{
public:
	void OutputDebugPrintf(const char* strOutputString, ...);
	CData();
	~CData();
	void RandDir(byte& dir) {
		int i = GetRand();
		switch (i % 25) {
		case 1:dir = 'W'; break;
		case 2:dir = 'A'; break;
		case 3:dir = 'S'; break;
		case 4:dir = 'D'; break;
		default:break; }
	};
	int GetRand(int min = 0, int max = RAND_MAX) {
		return min + rand() % max;
	};
	byte GetKey();
private:

};

#pragma region 注册宏定义
//判断指定的按键是否按下
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
//判断指定的按键是否弹起
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)

//定义地图长宽
#define MAP_W 30
#define MAP_H 27
#define TANKER_MAX 7

//定义热键
#define KEY_ESC 27

//定义游戏状态
#define de单人游戏闯关 0x01

//定义方向
#define de向上 'W'
#define de向左 'A'
//定义坦克属性
#define deCol敌方速度型 0x03
#define deCol敌方超重型 0x05
#pragma endregion
#pragma region 字符串索引
#define	deStr河流 "≈"
#define	deCol河流 0x0B
#define	deStr草坪 "※"
#define	deCol草坪 0x0A
#define	deStr子弹 "●"
#define	deStr土墙 "◇"
#define	deStr空地 "　"
const char MAPEnum[][5] = {
	deStr空地,"■",deStr土墙,deStr草坪,deStr河流 };
const WORD MColor[5] = {
	0x0F ,0x0F ,0x06,deCol草坪,deCol河流 };
#define	demap河流 0x04
#define	demap草坪 0x03
#define	demap土墙 0x02
#define	demap铁墙 0x01
#define	demap空地 0x00
const char INFOMenu[][31] = {
	{"欢迎使用CO0Kie丶初代坦克大战。"},
	{"①单人游戏"},
	{"②双人游戏"},
	{"③读取游戏"},
	{"④编辑地图"},
	{"⑤ 退 出"}
};
const char szTankSharp[2][4][3][7] = {
	{
		{"  ■  ","■■■","■  ■"},		//上
		{"  ■■","■■  ","  ■■"},		//左
		{"■■  ","  ■■","■■  "},		//右
		{"■  ■","■■■","  ■  "}		//下
	},
	{
		{"  □  ","□□□","□  □"},		//上
		{"  □□","□□  ","  □□"},		//左
		{"□□  ","  □□","□□  "},		//右
		{"□  □","□□□","  □  "}		//下
	}
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
	unsigned char TANKER : 3;
	unsigned char Wall : 1;
	unsigned char Earth : 1;
	unsigned char Rever : 1;
	unsigned char Grass : 1;
	unsigned char Bullet : 1;
}CHARMAP;

//定义游戏世界参数
#define 游戏 0x00
typedef struct _GAMEINFO {	//全局游戏信息
	byte menu = 0x01;		//记录菜单信息
	byte levels = 0x00;		//记录关卡
	time_t now = 0x00;		//声明time_t类型变量
	ULONGLONG start = 0x00;	//记录游戏开始时间
}GAMEINFO, * PGAMEINFO;
#pragma endregion
#pragma region 全局变量
extern CHARMAP map[MAP_H][MAP_W];			//全局地图
extern HANDLE gOUTPUT;						//窗口输出句柄
extern GAMEINFO gGINFO;
extern CData gAPI;
#pragma endregion