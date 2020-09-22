#pragma once
#include <ctime>
#include <Windows.h>
class CData
{
public:
	void OutputDebugPrintf(const char* strOutputString, ...);
	CData();
	~CData();
	void RandDir(byte& dir) {
		int i = GetRand();
		switch (i % 25) {
		case 1:dir = 'W'; break;
		case 2:dir = 'A'; break;
		case 3:dir = 'S'; break;
		case 4:dir = 'D'; break;
		default:break; }
	};
	int GetRand(int min = 0, int max = RAND_MAX) {
		return min + rand() % max;
	};
	byte GetKey();
private:

};

#pragma region ע��궨��
//�ж�ָ���İ����Ƿ���
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
//�ж�ָ���İ����Ƿ���
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)

//�����ͼ����
#define MAP_W 30
#define MAP_H 27
#define TANKER_MAX 7

//�����ȼ�
#define KEY_ESC 27

//������Ϸ״̬
#define de������Ϸ���� 0x01

//���巽��
#define de���� 'W'
#define de���� 'A'
//����̹������
#define deCol�з��ٶ��� 0x03
#define deCol�з������� 0x05
#pragma endregion
#pragma region �ַ�������
#define	deStr���� "��"
#define	deCol���� 0x0B
#define	deStr��ƺ "��"
#define	deCol��ƺ 0x0A
#define	deStr�ӵ� "��"
#define	deStr��ǽ "��"
#define	deStr�յ� "��"
const char MAPEnum[][5] = {
	deStr�յ�,"��",deStr��ǽ,deStr��ƺ,deStr���� };
const WORD MColor[5] = {
	0x0F ,0x0F ,0x06,deCol��ƺ,deCol���� };
#define	demap���� 0x04
#define	demap��ƺ 0x03
#define	demap��ǽ 0x02
#define	demap��ǽ 0x01
#define	demap�յ� 0x00
const char INFOMenu[][31] = {
	{"��ӭʹ��CO0Kieؼ����̹�˴�ս��"},
	{"�ٵ�����Ϸ"},
	{"��˫����Ϸ"},
	{"�۶�ȡ��Ϸ"},
	{"�ܱ༭��ͼ"},
	{"�� �� ��"}
};
const char szTankSharp[2][4][3][7] = {
	{
		{"  ��  ","������","��  ��"},		//��
		{"  ����","����  ","  ����"},		//��
		{"����  ","  ����","����  "},		//��
		{"��  ��","������","  ��  "}		//��
	},
	{
		{"  ��  ","������","��  ��"},		//��
		{"  ����","����  ","  ����"},		//��
		{"����  ","  ����","����  "},		//��
		{"��  ��","������","  ��  "}		//��
	}
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
	unsigned char TANKER : 3;
	unsigned char Wall : 1;
	unsigned char Earth : 1;
	unsigned char Rever : 1;
	unsigned char Grass : 1;
	unsigned char Bullet : 1;
}CHARMAP;

//������Ϸ�������
#define ��Ϸ 0x00
typedef struct _GAMEINFO {	//ȫ����Ϸ��Ϣ
	byte menu = 0x01;		//��¼�˵���Ϣ
	byte levels = 0x00;		//��¼�ؿ�
	time_t now = 0x00;		//����time_t���ͱ���
	ULONGLONG start = 0x00;	//��¼��Ϸ��ʼʱ��
}GAMEINFO, * PGAMEINFO;
#pragma endregion
#pragma region ȫ�ֱ���
extern CHARMAP map[MAP_H][MAP_W];			//ȫ�ֵ�ͼ
extern HANDLE gOUTPUT;						//����������
extern GAMEINFO gGINFO;
extern CData gAPI;
#pragma endregion