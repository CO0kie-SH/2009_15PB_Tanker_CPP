#include "CCtrl.h"
#include <conio.h>

//HANDLE gOUTPUT;

#pragma region ��ʼ����Ϸ����
CCtrl::CCtrl(COORD xy)
	:_maxXY(xy)
{
	PV = new CView();
	if (!PV) {
		printf_s("��ʼ����ͼ����\n");
		exit(1);
	}
	PM = new CMap();
	if (!PV) {
		printf_s("��ʼ����ͼ����\n");
		exit(1);
	}
	
	gOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
	gAPI.OutputDebugPrintf("��ʼ����");
}

CCtrl::~CCtrl()
{
	//�ͷ���ͼ��&��ͼ��
	delete PV;
	delete PM;
}

bool CCtrl::InitCMD()
{
	//��ʼ�����ڹ�ꡢ��С
	if (PV->InitWindow(_maxXY, false) == false)
		return false;
	return true;
}
#pragma endregion

//��Ϸ��ʼ
#pragma region ��ʼ����Ϸ����
int CCtrl::Go(int index)
{
	byte bGame = 'G', key = 0, i = 0, dir = 0;
	CHARMAP tmap = { 0 };
	COORD xy = { 0,0 };
	if (de������Ϸ���� == index) {
		PM->SetMap(1);
		this->_tanks = 3;
		this->cT[0].Init({ MAP_W / 2, MAP_H - 2 }, 100, 1, 'D', 'A', 0x04);
		tmap.TANKER = 1;
		PM->SetMap(cT[tmap.TANKER - 1].GetOldXY(), tmap, true);
		this->cT[1].Init({ 1, 1 }, 0, 1, 'D', 'A', 0x09);
		this->cT[2].Init({ MAP_W / 2 - 6, 1 }, 100, 2, 'S', 'B', 0x03);
		this->cT[3].Init({ MAP_W / 2 + 6, 1 }, 0, 2, 'S', 'B', 0x03);
		tmap.TANKER = 3;
		PM->SetMap(cT[tmap.TANKER - 1].GetOldXY(), tmap, true);
	}
#pragma endregion

//��ʼѭ����Ϸ
#pragma region ��ʼѭ����Ϸ
	PV->PrintMap();
	do
	{
		PV->PrintMap(cT[0]);
		tmap.TANKER = 0;
		for (byte i = 2; i < this->_tanks; i++) {
			PM->SetMap(cT[i].GetOldXY(), tmap, true);
			PV->PrintMap(cT[i], true);
		}
		//�ӵ�����
		for (byte i = 2; i < this->_tanks; i++) {
			if (!cT[i].IsAlive()) continue;
			tmap.TANKER = i + 1;
			do {
				dir = NULL;
				gAPI.RandDir(dir);
			} while (!cT[i].TryMove(dir));
			cT[i].Move();
			PM->SetMap(cT[i].GetOldXY(), tmap, true);
			PV->PrintMap(cT[i]);
		}
		key = gAPI.GetKey(); Sleep(100);		//ͨ��API���ؼ�ֵ����ʱ

		switch (key) {
		case 'W':case 'A':case 'S':case 'D':	//������ƶ�̹��1
			if (cT[0].TryMove(key) == true) {	//�ɹ��ƶ�
				tmap.TANKER = 0;				//��ʶ��ͼΪ̹��NULL
				PM->SetMap(cT[0].GetOldXY(), tmap, true);
				PV->PrintMap(cT[0], true);		//����ɵ�̹��
				cT[0].Move();					//��ֵ�µ�����
				tmap.TANKER = 1;				//��ʶ��ͼΪ���1
				PM->SetMap(cT[0].GetOldXY(), tmap, true);
				PV->PrintMap(cT[0]);			//��ӡ�µ�̹��
			} break;
		case 'F':
			this->AddBullet(cT[0]);
			break;
		case 'I':case 'J':case 'K':case 'L':	//������ƶ�̹��2
			break;
		case KEY_ESC: bGame = 0x00; break;
		default: break;
		}
	} while (bGame);

	memset(map, 0, MAP_H * MAP_W);				//������Ϸ������ʼ����ͼ
	return 0;
}

bool CCtrl::AddBullet(CTanker& cT)
{
	return true;
}
#pragma endregion
