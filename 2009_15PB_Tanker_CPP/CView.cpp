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
	//�����ı��С
	//SMALL_RECT srctWindow = { 0,0,xy.X,xy.Y };
	//if (!SetConsoleWindowInfo(gOUTPUT, true, &srctWindow))
	//{	//���ÿ���̨���ڴ�С
	//	printf_s("size err %d\n", GetLastError());
	//	return false;
	//}
	return (bool)SetConsoleTitleA(INFOMenu[0]);
}

void CView::PrintPoint(COORD xy, const char* text, WORD color)
{
	if (color != 0x00)							//���������ɫ
		SetConsoleTextAttribute(gOUTPUT, color);//��ı仭ˢ
	xy.X = xy.X * 2 + 2; xy.Y++;				//ת����������
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
	cout << text << flush;						//�������
	if (color != 0x00)
		SetConsoleTextAttribute(gOUTPUT, 0x0F);	//��ԭ��ˢ
}

void CView::PrintMap()
{
	unsigned short x = MAP_W + 1, y = 0;
	const char* sWall = MAPEnum[demap��ǽ];
	system("cls");
	while (x--)
		cout << sWall;
	for (y = 0; y < MAP_H; ++y)
	{
		printf_s("��%2d\n%s", y, sWall);
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
	unsigned char index = 0x00;		//��ʼ����ӡ����
	if (tmap.Rever == true) {		//����
		index = demap����;
	}else if (tmap.Wall == true) {	//��ǽ
		index = demap��ǽ;
	}else if (tmap.Grass == true) {	//�ݵ�
		index = demap��ƺ;
	}else if (tmap.Earth == true) {	//��ǽ
		index = demap��ǽ;
	}
	SetConsoleTextAttribute(gOUTPUT, MColor[index]);
	cout << MAPEnum[index];			//ͨ��������ӡ��Ӧ��ɫ�ַ�
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
	for (j = -1; j < 2; j++)		//��ƺʼ�ջ���
		for (i = -1; i < 2; i++)	//ѭ��̹������
			if (map[that._oxy.Y + j][that._oxy.X + i].Grass == true)
				this->PrintPoint(	//����ǲ�ƺ���򻭲�
					{ that._oxy.X + i,that._oxy.Y + j }, "��", 0x0A);
}
