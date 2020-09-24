#include "CAStar.h"
#include <algorithm>


void CAStar::Go()
{
	memcpy_s(tmap, MAP_LEN, map, MAP_LEN);
	_open.push_back({ _hxy.X,_hxy.Y });
	//Print(_exy, "β");
	if (FindOpen() && GetPath()) {
		//�ҵ�·��
		_find = (*(_path.end() - 1)).XY;
		//Print(_find, "��", 0x0A);
	}
	_open.clear(); _close.clear();
}

bool CAStar::FindOpen()
{
	COORDEX OldPos;
	COORDEX newPos;
	vector<COORD> points;
	char buff[250];
	while (!_open.empty())
	{
		auto min = min_element(_open.rbegin(), _open.rend(), compare);
		sprintf_s(buff, "%2d", (*min).g_value);
		Print((*min).XY, buff);
		Sleep(5);
		OldPos = (*min);					//��ȡ��
		this->tmap[OldPos.XY.Y][OldPos.XY.X].Wall = true;
		_close.push_back(OldPos);			//�������ҵı�
		_open.erase(--min.base());			//ɾ�����ҵı�
		points.clear();						//��ʼ���ص�����
		if (!KuoSan(OldPos.XY, points))		//ȡ��ɢ���飬���
			continue;						//û�У���ѭ��β

		for (auto& point : points) {		//ѭ����ɢ��
			newPos = COORDEX{ point.X,point.Y };
			newPos.Init(OldPos, _exy);		//��ʼ������
			if (newPos == _exy) {			//�������Ϊ�յ�
				_find = point;				//����ֵ
				_close.push_back(newPos);	//�����ѱ�
				return true;				//����ѭ��
			}
			Print(point, "��");
			_open.push_back(newPos);
		}
	}
	return false;
}

bool CAStar::KuoSan(COORD oXY, vector<COORD>& points)
{
	COORD offset[4] = { {-1, 0}, {1, 0}, {0,-1}, {0, 1} };
	COORD addPos;
	for (int i = 0; i < 4; ++i)
	{
		addPos = {
			offset[i].X + oXY.X,
			offset[i].Y + oXY.Y
		};
		//gAPI.OutputDebugPrintf("%d,P=%p == %d,%d,%d\n",
		//	i, &oXY, i, addPos.X, addPos.Y);
		if (Check2(addPos))
			points.push_back(addPos);
	}
	return !points.empty();
}

bool CAStar::Check2(COORD& tryXY)
{
	if (tryXY.X < 0 || tryXY.Y < 0)
		return false;
	else if (tryXY.X >= MAP_W || tryXY.Y >= MAP_H)
		return false;
	else {
		CHARMAP tmp = tmap[tryXY.Y][tryXY.X];
		if (tmp.TANKER || tmp.Wall)			//��ǽ��̹�˲��ܴ���
			return false;					//��ʶ����ֵ
	}
	tmap[tryXY.Y][tryXY.X].Wall = true;
	return true;
}

bool CAStar::GetPath()
{
	int i;
	for (i = _close.size() - 1; i >= 0; --i)
	{
		if (_close[i] == _hxy) break;
		if (_close[i] == _find) {
			Print(_find, "·");
			_path.push_back(_close[i]);
			_find = _close[i].oXY;
		}
	}
	return _close[i] == _hxy;
}

/*	���Ƶ�ͼ
	���ڻ��Ƶ�ͼ	*/
void CAStar::Print(COORD& xy, const char* text, WORD color)
{
	COORD pxy = { xy.X,xy.Y };
	if (color != 0x00)							//���������ɫ
		SetConsoleTextAttribute(gOUTPUT, color);//��ı仭ˢ
	pxy.X = pxy.X * 2 + 2; pxy.Y++;				//ת����������
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pxy);
 	printf_s("%s", text);
	if (color != 0x00)
		SetConsoleTextAttribute(gOUTPUT, 0x0F);	//��ԭ��ˢ
}

/*	��ʼ�����꺯��
	���ڼ�������ĵ�fghֵ	*/
void COORDEX::Init(const COORDEX& oXY, COORD& END)
{
	this->oXY = oXY.XY;
	this->g_value = oXY.g_value + 1;
	this->h_value = abs(XY.X - END.X) + abs(XY.Y - END.Y);
	this->f_value = this->g_value + this->h_value;
}
