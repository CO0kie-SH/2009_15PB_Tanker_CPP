#pragma once

#include "CTanker.h"
#include <iostream>
using std::cout;using std::endl;

class CView
{
public:
	CView();
	~CView();

	void PrintTank(CTanker& that);
	bool InitWindow(COORD& xy, bool isCursor);
private:

};