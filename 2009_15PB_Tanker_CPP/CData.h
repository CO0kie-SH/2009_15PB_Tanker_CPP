#pragma once
#include <ctime>
#include <Windows.h>

class CData
{
public:
	/*API����
	������VS���������Ϣ	*/
	void OutputDebugPrintf(const char* strOutputString, ...);

	CData();	//���캯��
	~CData();	//��������

	/*��������������ڸ����������*/
	void RandDir(byte& dir) {
		int i = GetRand();
		switch (i % 25) {
		case 1:dir = 'W'; break;
		case 2:dir = 'A'; break;
		case 3:dir = 'S'; break;
		case 4:dir = 'D'; break;
		default:dir = NULL; break; }
	};

	/*���������������ȡ�����*/
	int GetRand(int min = 0, int max = RAND_MAX) {
		return min + rand() % max;
	};
public:
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
#define MAP_LEN (MAP_W * MAP_H)

#define TANKER_MAX 7

//�����ȼ�
#define KEY_ESC 27

//������Ϸ״̬
#define de������Ϸ���� 0x01
#define de˫����Ϸ���� 0x02
#define de�Զ����ͼ 0x03
#define de�浵��Ϸ 0x04
#define de��ӡ��Ϸ��Ϣ 0x07

//���巽��
#define de���� 'W'
#define de���� 'A'
//����̹������
#define deCol�з��ٶ��� 0x03
#define deCol�з������� 0x05
// ͨ��ǰ��ɫ
#define COLOR_NO			0x0000
#define COLOR_H_GREEN		0x0008 | 0x0002
#define COLOR_H_BLUE		0x0008 | 0x0001
#define COLOR_H_YELLOW		0x0008 | 0x0004 | 0x0002
#define COLOR_WHITE			0x0004 | 0x0002 |0x0001
#define COLOR_H_WHITE		0x0008 | 0x0004 | 0x0002 |0x0001
#pragma endregion
#pragma region �ַ�������
//�����ͼ����
#define	deStr���� "��"
#define	deCol���� 0x0B
#define	deStr��ƺ "��"
#define	deCol��ƺ 0x0A
#define	deStr�ӵ� "��"
#define	deStr��ǽ "��"
#define	deStr��ǽ "��"
#define	deCol��ǽ 0x06
#define	deStr�յ� "��"
const char MAPEnum[][5] = {
	deStr�յ�,"��",deStr��ǽ,deStr��ƺ,deStr���� };
const WORD MColor[8] = {
	0x0F ,0x0F ,deCol��ǽ,deCol��ƺ,deCol����,0x0F,0x06,0x0E };
#define	demap���� 0x04
#define	demap��ƺ 0x03
#define	demap��ǽ 0x02
#define	demap��ǽ 0x01
#define	demap�յ� 0x00
#define	deMenu 0x06
const char INFOMenu[][33] = {
	{"��ӭʹ��CO0Kieؼ̹�˴�սCPP1.0"},
	{"�ٵ��˴�����Ϸ"},
	{"��˫�˴�����Ϸ"},
	{"���� �� ���ͼ"},
	{"���� �� �� ��"},
	{"���� �� �� Ϸ"},
	{"������������������"}
};
const char VIEWINFO[][31] = {
	{"��������ѡ�񣬰��ո��ѡ��"},
	{"��ϵ���߿�ͨ�����ո��ѡ��"}
};
const char cstr��ͣ[][31] = {
	{"��Ϸ��ͣ������������"},
	{"�� Y�ɴ�����Ϸ������"},
	{"�� N�ɼ�����Ϸ������"},
	{"��������������������"},
	{"��������������������"}
};
const char szKEYs[][17] = {
	"��ס W���ɻ���ǽ",
	"��ס D���ɻ��յ�",
	"��ס F���ɻ���ǽ",
	"��ס G���ɻ���ƺ",
	"��ס R���ɻ�����",
	"����������������",
	"��ס H�����ؽ���",
	"��ס J��ȷ�ϵ�ͼ"
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
const char PlayerInfo2[][20] = {
	{"Ѫ��\t  %-5d\t%-5d"},
	{"�ٶ�\t  %-5d\t%-5d"},
	{"������  %-5d\t%-5d"},
	{"ɱ����  %-5d\t%-5d"},
	{"��  ��  %-5d\t%-5d"},
	{"��  ��  100ʱ�ɽ���"}
};
const char PlayerInfo[][20] = {
	{"Ѫ��\t  %-5d"},
	{"�ٶ�\t  %-5d"},
	{"������  %-5d"},
	{"ɱ����  %-5d"},
	{"��  ��  %-5d"},
	{"��  ��  100ʱ�ɽ���"}
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
typedef struct _GAMEINFO {			//ȫ����Ϸ��Ϣ
	byte player = 0x01;				//��¼�˵���Ϣ
	byte count = 0x07;				//��¼�˵���Ϣ
	byte menu = 0x01;				//��¼�˵���Ϣ
	byte levels = 0x00;				//��¼�ؿ�
	time_t now = 0x00;				//����time_t���ͱ���
	ULONGLONG start = 0x00;			//��¼��Ϸ��ʼʱ��
}GAMEINFO, * PGAMEINFO;
#pragma endregion
#pragma region ȫ�ֱ���
extern CHARMAP map[MAP_H][MAP_W];	//ȫ�ֵ�ͼ
extern HANDLE gOUTPUT;				//����������
extern GAMEINFO gGINFO;				//������Ϸ��Ϣ
extern CData gAPI;					//����ȫ��API
#pragma endregion