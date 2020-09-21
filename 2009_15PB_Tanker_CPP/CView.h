#pragma once

#include "CTanker.h"
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
	void PrintPoint(COORD xy, const char* text,
		WORD color = 0x00, bool setMap = true);
	void PrintMap();
	void PrintMap(CHARMAP tmap);
	void PrintMap(CTanker& that);
private:

};