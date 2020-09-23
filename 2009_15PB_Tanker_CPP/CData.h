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
		default:dir = NULL; break; }
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
#define de双人游戏闯关 0x02
#define de自定义地图 0x03

//定义方向
#define de向上 'W'
#define de向左 'A'
//定义坦克属性
#define deCol敌方速度型 0x03
#define deCol敌方超重型 0x05
#pragma endregion

#pragma region 字符串索引
//定义地图属性
#define	deStr河流 "≈"
#define	deCol河流 0x0B
#define	deStr草坪 "※"
#define	deCol草坪 0x0A
#define	deStr子弹 "●"
#define	deStr铁墙 "■"
#define	deStr土墙 "◇"
#define	deCol土墙 0x06
#define	deStr空地 "　"
const char MAPEnum[][5] = {
	deStr空地,"■",deStr土墙,deStr草坪,deStr河流 };
const WORD MColor[8] = {
	0x0F ,0x0F ,deCol土墙,deCol草坪,deCol河流,0x0F,0x06,0x0E };
#define	demap河流 0x04
#define	demap草坪 0x03
#define	demap土墙 0x02
#define	demap铁墙 0x01
#define	demap空地 0x00
#define	deMenu 0x06
const char INFOMenu[][31] = {
	{"欢迎使用CO0Kie丶初代坦克大战。"},
	{"①单人闯关游戏"},
	{"②双人闯关游戏"},
	{"③自 定 义地图"},
	{"④载 入 存 档"},
	{"⑤退 出 游 戏"},
	{"　　　　　　　　　"}
};
const char VIEWINFO[][31] = {
	{"上下左右选择，按空格键选择。"},
	{"联系作者开通，按空格键选择。"}
};
const char szKEYs[][17] = {
	"按住 W键可画铁墙",
	"按住 D键可画空地",
	"按住 F键可画土墙",
	"按住 G键可画草坪",
	"按住 R键可画河流",
	"　　　　　　　　",
	"按住 H键返回界面",
	"按住 J键确认地图"
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
const char PlayerInfo2[][20] = {
	{"血量\t  %-5d\t%-5d"},
	{"速度\t  %-5d\t%-5d"},
	{"攻击力  %-5d\t%-5d"},
	{"杀敌数  %-5d\t%-5d"},
	{"得  分  %-5d\t%-5d"},
	{"满  分  100时可进阶"}
};
const char PlayerInfo[][20] = {
	{"血量\t  %-5d"},
	{"速度\t  %-5d"},
	{"攻击力  %-5d"},
	{"杀敌数  %-5d"},
	{"得  分  %-5d"},
	{"满  分  100时可进阶"}
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
typedef struct _GAMEINFO {			//全局游戏信息
	byte player = 0x01;				//记录菜单信息
	byte count = 0x08;				//记录菜单信息
	byte menu = 0x01;				//记录菜单信息
	byte levels = 0x00;				//记录关卡
	time_t now = 0x00;				//声明time_t类型变量
	ULONGLONG start = 0x00;			//记录游戏开始时间
}GAMEINFO, * PGAMEINFO;
#pragma endregion
#pragma region 全局变量
extern CHARMAP map[MAP_H][MAP_W];	//全局地图
extern HANDLE gOUTPUT;				//窗口输出句柄
extern GAMEINFO gGINFO;				//定义游戏信息
extern CData gAPI;					//定义全局API
#pragma endregion