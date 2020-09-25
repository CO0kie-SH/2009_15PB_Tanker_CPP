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
	CBgm	PB;	//����BGM

	bool InitWindow(COORD& xy, bool isCursor);	//��ʼ������
	void PrintPoint(COORD xy, const char* text, WORD color = 0x00);
	void PrintMap();							//ˢ�´��ڡ��滭��ͼ
	void PrintMap(CHARMAP tmap);				//���ػ滭��ͼ��
	void PrintMap(CTanker& that, bool clean = false);	//���ػ滭̹��
	void PrintMap(byte menuIndex, bool err = false);	//���ػ�����Ϸ��Ϣ
	bool SaveGame(CTanker* Tanks);				//������Ϸ
	bool ReadGame(CTanker* Tanks);				//��ȡ��Ϸ
private:

};