#pragma once
#include <Windows.h>

class CData
{
public:
	void OutputDebugPrintf(const char* strOutputString, ...);
	CData();
	~CData();

private:

};

#pragma region ע��궨��

//�����ͼ����
#define MAP_W 30
#define MAP_H 27
#define TANKER_MAX 15

//�����ȼ�
#define KEY_ESC 27

#pragma endregion

#pragma region �ַ�������
const char INFOMenu[][31] = {
	{"��ӭʹ��CO0Kieؼ����̹�˴�ս��"},
	{"�ٵ�����Ϸ"},
	{"��˫����Ϸ"},
	{"�۶�ȡ��Ϸ"},
	{"�ܱ༭��ͼ"},
	{"�� �� ��"}
};
const char GAMEDesc[][20] = {
	{"���\t��� 1\t��� 2"},
	{"����\t  W\t  I"},
	{"����\t  A\t  J"},
	{"����\t  S\t  K"},
	{"����\t  D\t  L"},
	{"����\t  F\t  ;"}
};
const char PlayerInfo[][15] = {
	{"Ѫ��\t  %d\t%d"},
	{"�ٶ�\t  %d\t%d"},
	{"������  %d\t%d"},
	{"ɱ����  %d\t%d"},
	{"��  ��  %d\t%d"}
};
#pragma endregion


#pragma region ����ṹ��
//�����ͼ
typedef struct _CHARMAP {
	unsigned char TANKER : 4;
	unsigned char WALL : 1;
	unsigned char BLOCK : 1;
	unsigned char BULLET : 1;
}CHARMAP;

//������Ϸ�������
#define ��Ϸ 0x00
typedef struct _GAMEINFO {
	byte menu = 0x01;
	byte statu = 0x00;
	time_t now;								//����time_t���ͱ���
	ULONGLONG start;
}GAMEINFO, * PGAMEINFO;
#pragma endregion


#pragma region ȫ�ֱ���
extern CHARMAP map[MAP_H][MAP_W];			//ȫ�ֵ�ͼ
extern HANDLE gOUTPUT;						//����������
extern PGAMEINFO gGINFO;
extern CData gAPI;
#pragma endregion