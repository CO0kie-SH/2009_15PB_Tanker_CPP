#pragma once
#include "CTanker.h"
class CMap
{
public:
	/*���˵���ʼ����ͼ*/
	bool SetMap(int index = 1);

	/*��������Ϣ�ı��ͼ*/
	bool SetMap(COORD xy, CHARMAP stmap, bool isTank = false);
};