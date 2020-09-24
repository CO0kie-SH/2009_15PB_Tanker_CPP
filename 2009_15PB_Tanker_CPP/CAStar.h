#pragma once
#include "CData.h"
#include <vector>
using std::vector;


class COORDEX
{
public:
	COORD XY = { 0 };	// �����ӽڵ������
	COORD oXY = { 0 };	// ���游�ڵ������
	int h_value = 0;		// ��ǰ�㵽�յ������·�߲���
	int g_value = 0;		// ����㵽��ǰ���߹��Ĳ���
	int f_value = 0;		// f = g + h; ���۲���
	COORD GetPos() { return XY; };
	void Init(const COORDEX& oXY, COORD& END);

	// �ȽϺ��������ڱȽ������ڵ�ֵ�Ƿ����
	bool operator==(const COORD& pos)
	{
		return XY.X == pos.X && XY.Y == pos.Y;
	}
};

// Ҫ��������Ϊ�ص�����(��Ϊ�������д��ݵĺ���)�Ĳ������Ǿ�̬����
static bool compare(const COORDEX& point1, const COORDEX& point2)
{
	return point1.f_value < point2.f_value;
}

class CAStar
{
public:
	void Go();
	bool FindOpen();
	bool KuoSan(COORD oXY, vector<COORD>& points);
	bool Check2(COORD& tryXY);
	bool GetPath();
private:
	void Print(COORD& xy, const char* text, WORD color = 0x00);
	CHARMAP tmap[MAP_H][MAP_W];
	COORD _hxy = { 1,1 };
	COORD _exy = { 20,9 };
	COORD _find;
	// ����ȴ����ҵ�����λ��
	vector<COORDEX> _open;

	// �����Ѿ����ҵ�����λ��
	vector<COORDEX> _close;

	// �������·��
	vector<COORDEX> _path;
};

