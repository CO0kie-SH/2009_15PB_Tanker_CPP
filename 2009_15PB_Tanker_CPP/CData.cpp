#include "CData.h"
#include "atlbase.h"
#include "atlstr.h"
CHARMAP map[MAP_H][MAP_W];
void CData::API_OutputDebugPrintf(const char* strOutputString, ...)
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
}

CData::~CData()
{
}