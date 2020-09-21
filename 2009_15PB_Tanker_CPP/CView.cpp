#include "CView.h"

CView::CView()
{
}

CView::~CView()
{
}

bool CView::InitWindow(COORD& xy, bool isCursor)
{
	CONSOLE_CURSOR_INFO cursor = { 1,isCursor };
	char buf[50] = {0};
	SetConsoleCursorInfo(gOUTPUT, &cursor);
	sprintf_s(buf, "mode con cols=%d lines=%d", xy.X, xy.Y);
	system(buf);
	system("Color 0F");
	//函数改变大小
	//SMALL_RECT srctWindow = { 0,0,xy.X,xy.Y };
	//if (!SetConsoleWindowInfo(gOUTPUT, true, &srctWindow))
	//{	//设置控制台窗口大小
	//	printf_s("size err %d\n", GetLastError());
	//	return false;
	//}
	return (bool)SetConsoleTitleA(INFOMenu[0]);
}

void CView::PrintPoint(COORD xy, const char* text, WORD color)
{
	if (color != 0x00)							//如果传入颜色
		SetConsoleTextAttribute(gOUTPUT, color);//则改变画刷
	xy.X = xy.X * 2 + 2; xy.Y++;				//转换窗口坐标
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
	cout << text << flush;						//输出文字
	if (color != 0x00)
		SetConsoleTextAttribute(gOUTPUT, 0x0F);	//还原画刷
}

void CView::PrintMap()
{
	unsigned short x = MAP_W + 1, y = 0;
	const char* sWall = MAPEnum[demap铁墙];
	system("cls");
	while (x--)
		cout << sWall;
	for (y = 0; y < MAP_H; ++y)
	{
		printf_s("后%2d\n%s", y, sWall);
		for (x = 0; x<MAP_W; ++x)
		{
			PrintMap(map[y][x]);
		}
	}
	cout << sWall << '\n'; x += 2;
	while (x--)
		cout << sWall;
	cout << '\n' << sWall;
	for (x = 0; x < MAP_W; x++)
	{
		printf_s("%2d", x + 1);
	}
}

void CView::PrintMap(CHARMAP tmap)
{
	unsigned char index = 0x00;		//初始化打印索引
	if (tmap.Rever == true) {		//河流
		index = demap河流;
	}else if (tmap.Wall == true) {	//铁墙
		index = demap铁墙;
	}else if (tmap.Grass == true) {	//草地
		index = demap草坪;
	}else if (tmap.Earth == true) {	//土墙
		index = demap土墙;
	}
	SetConsoleTextAttribute(gOUTPUT, MColor[index]);
	cout << MAPEnum[index];			//通过索引打印相应颜色字符
	SetConsoleTextAttribute(gOUTPUT, 0x0F);
}

void CView::PrintMap(CTanker& that, bool clean)
{
	if (!that.IsAlive()) return;
	SHORT i, dir = that.GetDir(), j;
	for (i = 0; i < 3; i++)
	{
		this->PrintPoint({ that._oxy.X - 1,that._oxy.Y + i - 1 },
			clean ? "      " : szTankSharp[that._team - 'A'][dir][i],
			that._color);
	}
	for (j = -1; j < 2; j++)		//草坪始终画草
		for (i = -1; i < 2; i++)	//循环坦克身体
			if (map[that._oxy.Y + j][that._oxy.X + i].Grass == true)
				this->PrintPoint(	//如果是草坪，则画草
					{ that._oxy.X + i,that._oxy.Y + j }, "草", 0x0A);
}
