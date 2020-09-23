#pragma once

#include "CTanker.h"
#include "CBullet.h"
#include <iostream>
using std::cout;
using std::endl;
using std::flush;

class CView
{
public:
	CView();
	~CView();

	bool InitWindow(COORD& xy, bool isCursor);
	void PrintPoint(COORD xy, const char* text, WORD color = 0x00);
	void PrintMap();
	void PrintMap(CHARMAP tmap);
	void PrintMap(CTanker& that, bool clean = false);
	void PrintMap(byte menuIndex, bool err = false);
	bool SaveGame(CTanker* Tanks);
	bool ReadGame(CTanker* Tanks);
private:

};