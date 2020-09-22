#include "CCtrl.h"
#include <conio.h>

//HANDLE gOUTPUT;

#pragma region ��ʼ����Ϸ����
CCtrl::CCtrl(COORD xy)
	:_maxXY(xy)
{
	PV = new CView(); PM = new CMap();
	if (!PV || !PM) {
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
		this->_tanks = 4;
		this->cT[0].Init({ MAP_W / 2, MAP_H - 2 }, 100, 1, 'D', 'A', 0x04);
		this->cT[1].Init({ 1, 1 }, 0, 1, 'D', 'A', 0x09);
		this->cT[2].Init({ MAP_W / 2 - 6, 1 }, 100, 2, 'S', 'B', deCol�з��ٶ���);
		this->cT[3].Init({ MAP_W / 2 + 6, 1 }, 100, 2, 'S', 'B', deCol�з�������);
		tmap.TANKER = 1;
		PM->SetMap(cT[tmap.TANKER - 1].GetOldXY(), tmap, true);
	}
#pragma endregion

//��ʼѭ����Ϸ
#pragma region ��ʼѭ����Ϸ
	PV->PrintMap();
	PV->PrintMap(cT[0]);
	do
	{

#pragma region AI�����߼���
		tmap.TANKER = 0;
		for (i = 2; i < this->_tanks; i++) {
			PM->SetMap(cT[i].GetOldXY(), tmap, true);
			PV->PrintMap(cT[i], true);
		}
		//�ӵ�����
		auto begin = _bullets.begin();
		while (begin != _bullets.end()) {
			MoveBullet(*begin, true); ++begin;
		}
		MoveTank();
		begin = _bullets.begin();
		while (begin != _bullets.end()) {
			if ((*begin).IsAlive()) {
				++begin;
			}else begin = _bullets.erase(begin);
		}
#pragma endregion
#pragma region �˹������߼���
		key = gAPI.GetKey();					//ͨ��API���ؼ�ֵ����ʱ

		switch (key) {
		case 'W':case 'A':case 'S':case 'D':	//������ƶ�̹��1
			tmap.TANKER = 0;					//��ʶ��ͼΪ̹��NULL
			PM->SetMap(cT[0].GetOldXY(), tmap, true);
			if (cT[0].TryMove(key) == true) {	//�ɹ��ƶ�
				PV->PrintMap(cT[0], true);		//����ɵ�̹��
				cT[0].Move();					//��ֵ�µ�����
				tmap.TANKER = 1;				//��ʶ��ͼΪ���1
				PM->SetMap(cT[0].GetOldXY(), tmap, true);
				PV->PrintMap(cT[0]);			//��ӡ�µ�̹��
			} else {
				tmap.TANKER = 1;				//��ʶ��ͼΪ̹��1
				PM->SetMap(cT[0].GetOldXY(), tmap, true);
			} break;
		case 'F': this->AddBullet(0x01); break;
		case 'I':case 'J':case 'K':case 'L':	//������ƶ�̹��2
			break;
		case KEY_ESC: bGame = 0x00; break;
		default: break;
		}
#pragma endregion
		Sleep(100);
	} while (bGame);
	memset(map, 0, MAP_H * MAP_W);				//������Ϸ������ʼ����ͼ
	return 0;
}
#pragma endregion

bool CCtrl::AddBullet(byte tid)
{
	CTanker& tank = this->cT[tid-1];
	CBullet att;
	if(att.Init(tank.GetOldXY(), tank.GetDir(false),tank.GetTid())){
		if (MoveBullet(att))
			_bullets.push_back(att);
	}
	return true;
}

bool CCtrl::MoveBullet(CBullet& att, bool clean)
{
	COORD xy = att.GetOldXY();
	CHARMAP tmap = map[xy.Y][xy.X];
	if (clean) {
		map[xy.Y][xy.X].Bullet = false;	//������뻹ԭ�ӵ������ػ�
		if (tmap.Rever == true) {		//��ԭ��ɫ����
			PV->PrintPoint(xy, deStr����, deCol����);
		}
		else if (tmap.Grass == true) {	//��ԭ��ɫ��ƺ
			PV->PrintPoint(xy, deStr��ƺ, deCol��ƺ);
		}
		else {
			PV->PrintPoint(xy, deStr�յ�);
		}
	}
	if (att.TryMove()) {				//��ͼ�ƶ��ӵ�
		xy = att.Move();				//����ɹ������ƶ�
		tmap = map[xy.Y][xy.X];			//��ˢ�µ�ͼ
		if (tmap.Rever == true) {		//��������ɫ���ӵ�
			PV->PrintPoint(xy, deStr�ӵ�, deCol����);
		}
		else if (tmap.Grass == true) {	//��ƺ���ػ�

		}
		else if (tmap.Earth == true) {	//��ǽ������
			att.SetBlood(0);			//�����ӵ�
			tmap = { 0 };
			PM->SetMap(xy, tmap);		//���õ�ͼ
			PV->PrintPoint(xy, deStr�յ�);//���յ�
			return false;
		}
		else {
			PV->PrintPoint(xy, deStr�ӵ�);//���ӵ�
		}
	}
	else return false;		//�����ͼ�ƶ�ʧ�ܣ��򷵻ؼ�
	return true;
}

bool CCtrl::MoveTank()
{
	CHARMAP tmap = { 0 };
	byte dir, i;
	for (i = 2; i < this->_tanks; i++) {
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
	return false;
}

void CCtrl::PrintGInfo()
{

}

