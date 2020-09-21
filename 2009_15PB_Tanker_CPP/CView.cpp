#include "CView.h"

CView::CView()
{
}

CView::~CView()
{
}

void CView::PrintTank(CTanker& that)
{
	SHORT i = 0,j = 0;
	printf_s("Tid=%d\n", that._tid);
	for (i = -1; i < 2; i++)
	{
		for (j = -1; j < 2; j++)
		{
			printf_s("%d,%d\t", that._oxy.X + j, that._oxy.Y + i);
		}
		cout << "\n";
	}
	cout << endl;
}

bool CView::InitWindow(COORD& xy, bool isCursor)
{
	CONSOLE_CURSOR_INFO cursor = { 1,isCursor };
	SetConsoleTitleA(INFOMenu[0]);
	SetConsoleCursorInfo(gOUTPUT, &cursor);
	SMALL_RECT srctWindow = { 0,0,xy.X,xy.Y };
	if (!SetConsoleWindowInfo(gOUTPUT, true, &srctWindow))
	{	//设置控制台窗口大小
		printf_s("size err %d\n", GetLastError());
		return false;
	}
	return false;
}
