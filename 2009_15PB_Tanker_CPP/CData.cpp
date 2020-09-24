#include "CData.h"
#include <atlbase.h>
#include <atlstr.h>

CHARMAP map[MAP_H][MAP_W];	//全局地图
HANDLE gOUTPUT;				//窗口输出句柄
GAMEINFO gGINFO;			//定义游戏信息
CData gAPI;					//定义全局API

void CData::OutputDebugPrintf(const char* strOutputString, ...)
{
	char strBuffer[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
	_vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
	//vsprintf(strBuffer, strOutputString, vlArgs);
	va_end(vlArgs);
	OutputDebugString(CA2W(strBuffer));
}

CData::CData()
{
	memset(map, 0, MAP_H * MAP_W);
	srand((unsigned int)time(NULL));
	gGINFO.menu = 0x00;
	gGINFO.levels = 0x01;
}

CData::~CData()
{
}

byte CData::GetKey()
{
	if (KEY_DOWN('Y'))return 'Y';
	else if (KEY_DOWN('P')) return 'P';
	else if (KEY_DOWN('N')) return 'N';
	else if (KEY_DOWN('R')) return 'R';
	else if (KEY_DOWN('G')) return 'G';
	else if (KEY_DOWN('F')) return 'F';
	else if (KEY_DOWN('H')) return 'H';
	else if (KEY_DOWN('W')) return 'W';
	else if (KEY_DOWN('A')) return 'A';
	else if (KEY_DOWN('S')) return 'S';
	else if (KEY_DOWN('D')) return 'D';
	else if (KEY_DOWN('I')) return 'I';
	else if (KEY_DOWN('J')) return 'J';
	else if (KEY_DOWN('K')) return 'K';
	else if (KEY_DOWN('L')) return 'L';
	else if (KEY_DOWN(KEY_ESC)) return KEY_ESC;
	return NULL;
}
