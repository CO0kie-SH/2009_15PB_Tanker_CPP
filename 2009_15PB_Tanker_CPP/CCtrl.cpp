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
	byte menu = 1;
	PV->PrintMap(); PV->PrintMap(menu);
	do
	{
		PV->PrintMap(menu);
		switch (_getch()) {
		case 'W': case 'I': case 'w': case 'i':
			if (--menu < 1) menu = 5; break;
		case 'S': case 'K': case 's': case 'k':
			if (++menu > 5) menu = 1; break;
		case KEY_ESC:
			if (menu == 5) return false;
			menu = 5; break;
		case 32: {
			if (menu == 5) return false;
			else if (menu >= 1 && menu <= 2)
				Go(menu, gGINFO.levels);
			else if (menu == 3)
				this->DrawMap();
			else
				PV->PrintMap(menu, true);
			break; }
		default: break;
		}
	} while (true);
	return false;
}
#pragma endregion

//��Ϸ��ʼ
#pragma region ��ʼ����Ϸ����
//��������̹��
void CCtrl::SetTank(int TankI)
{
	CHARMAP tmap = { 0 };
	if (2 == TankI)
		this->cT[2].Init({ MAP_W / 2 - 6, 1 }, 50,
			3, 'S', 'B', deCol�з��ٶ���, 100);
	else if (3 == TankI)
		this->cT[3].Init({ MAP_W / 2 + 6, 1 }, 60,
			4, 'S', 'B', deCol�з�������, 300, 51);
	else if (0 == TankI) {
		this->cT[0].Init({ MAP_W / 2, MAP_H - 2 }, 100,
			1, 'D', 'A', 0x04);
		tmap.TANKER = TankI + 1;
		PM->SetMap(cT[TankI].GetOldXY(), tmap, true);
	}
	else if (1 == TankI) {
		this->cT[1].Init({ MAP_W / 2 + 4, MAP_H - 2 }, 100,
			2, 'W', 'A', 0x09);
		tmap.TANKER = TankI + 1;
		PM->SetMap(cT[TankI].GetOldXY(), tmap, true);
	}
}

int CCtrl::Go(int GameMode, int Checkpoint)
{
	byte bGame = 'G', key = 0, i = 0;
	COORD xy = { 0,0 }; CHARMAP tmap = { 0 };
	unsigned int msecond = 0;
	if (de������Ϸ���� == GameMode) {		//��ʼ��������Ϸ
		PM->SetMap(Checkpoint);
		gGINFO.player = 1;
		this->_tanks = 4;
		this->SetTank(0);
		this->cT[1].Init({ 1, 1 }, 0, 2, 'D', 'A', 0x09);
		this->SetTank(2);
		this->SetTank(3);
	}
	else if (de˫����Ϸ���� == GameMode) {	//��ʼ��˫����Ϸ
		PM->SetMap(Checkpoint);
		gGINFO.player = 2;
		this->_tanks = 4;
		for (i = 0; i <= this->_tanks; i++)
			this->SetTank(i);
	}
	gGINFO.start = GetTickCount64();	//�洢��ǰʱ��
#pragma endregion

//��ʼѭ����Ϸ
#pragma region ��ʼѭ����Ϸ
	PV->PrintMap();
	PV->PrintMap(cT[0]);
	if(gGINFO.player==2) PV->PrintMap(cT[1]);
	do {
		msecond = (unsigned int)(
			GetTickCount64() - gGINFO.start);
		PrintGInfo(msecond);
#pragma region AI�����߼���
		tmap.TANKER = 0;
		for (i = 2; i < this->_tanks; i++) {
			PM->SetMap(cT[i].GetOldXY(), tmap, true);
			PV->PrintMap(cT[i], true);
		}
		//�ӵ�����
		auto begin = _bullets.begin();
		while (begin != _bullets.end()) {
			MoveBullet(*begin, true);
			++begin;
		}
		MoveTank(msecond);
		begin = _bullets.begin();
		while (begin != _bullets.end()) {
			if ((*begin).IsAlive()) {
				++begin;
			}else begin = _bullets.erase(begin);
		}
#pragma endregion
#pragma region �˹������߼���
		key = gAPI.GetKey();					//ͨ��API���ؼ�ֵ
		switch (key) {
		case 'W':case 'A':case 'S':case 'D':	//������ƶ�̹��1
			//���	̹���ٶ�>(��Ϸʱ��-̹���ϴ�ʱ��)���ӡ����
			if (cT[0].GetSpeed() > (int)(msecond - cT[0].mSecond)) {
				break;
			}	cT[0].mSecond = msecond;
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
		case 'H': this->AddBullet(0x02); break;
		case 'I':case 'J':case 'K':case 'L':	//������ƶ�̹��2
			//���	̹���ٶ�>(��Ϸʱ��-̹���ϴ�ʱ��)���ӡ����
			if (cT[1].GetSpeed() > (int)(msecond - cT[1].mSecond)) {
				break;
			}	cT[1].mSecond = msecond;
			tmap.TANKER = 0;					//��ʶ��ͼΪ̹��NULL
			PM->SetMap(cT[1].GetOldXY(), tmap, true);
			if (cT[1].TryMove(key) == true) {	//�ɹ��ƶ�
				PV->PrintMap(cT[1], true);		//����ɵ�̹��
				cT[1].Move();					//��ֵ�µ�����
				tmap.TANKER = 2;				//��ʶ��ͼΪ���2
				PM->SetMap(cT[1].GetOldXY(), tmap, true);
				PV->PrintMap(cT[1]);			//��ӡ�µ�̹��
			}
			else {
				tmap.TANKER = 2;				//��ʶ��ͼΪ̹��2
				PM->SetMap(cT[2].GetOldXY(), tmap, true);
			} break;
		case KEY_ESC: bGame = 0x00; break;
		default: break;
		}
		for (i = 0; i < gGINFO.player; i++)
		{
			xy = cT[i].GetOldXY();
			this->CheckTank(xy, i + 1);
			if (!cT[i].IsAlive()) {
				bGame = NULL;
				break;
			}
		}
#pragma endregion
		Sleep(78);
	} while (bGame);
	msecond = (unsigned int)(
		GetTickCount64() - gGINFO.start);
	gGINFO.count = 8;
	this->PrintGInfo(msecond, 0x0C);
	PV->PrintPoint({ MAP_W / 2 - 6,MAP_H / 2 },
		"��Ϸ��������Y�˳���", 0x0C);
	if (key != KEY_ESC) printf_s("���%d������", i + 1);
	while ((i = _getch())) {
		if (i == 'Y' || i == 'y')
			break;
		Sleep(55);
	}
	memset(map, 0, MAP_H * MAP_W);				//������Ϸ������ʼ����ͼ
	PV->PrintMap(); gGINFO.menu = 0x00;
	return 0;
}
#pragma endregion


//̹�ˡ��ӵ�������
#pragma region ̹���ӵ�������

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
		else if(tmap.Bullet == true){		//�ӵ����ӵ���ײ
			map[xy.Y][xy.X].Bullet = false;	//���õ�ͼ
			PV->PrintPoint(xy, deStr�յ�);	//���յ�
			att.SetBlood(0);				//��������
			FindBullet(xy, nullptr);		//�ҵ��ӵ��㲢����
			return	false;
		}
		else {
			map[xy.Y][xy.X].Bullet = true;//���õ�ͼ
			PV->PrintPoint(xy, deStr�ӵ�);//���ӵ�
		}
	}
	else return false;		//�����ͼ�ƶ�ʧ�ܣ��򷵻ؼ�
	return true;
}

bool CCtrl::MoveTank(unsigned int& mSecond)
{
	CHARMAP tmap = { 0 };
	COORD xy = { 0,0 };
	byte dir, i;
	for (i = 2; i < this->_tanks; i++) {//ѭ��̹��
		if (!cT[i].IsAlive()) continue;	//���������������
		tmap.TANKER = i + 1;			//����ͼ����Ϊ̹��
		//���	̹���ٶ�>(��Ϸʱ��-̹���ϴ�ʱ��)���ӡ����
		if (cT[i].GetSpeed() > (int)(mSecond - cT[i].mSecond)) {
			PM->SetMap(cT[i].GetOldXY(), tmap, true);
			PV->PrintMap(cT[i]); continue;
		}	cT[i].mSecond = mSecond;
		do {							//ѭ���ƶ�̹��
			gAPI.RandDir(dir);			//ͨ��API��1/24��%
		} while (!cT[i].TryMove(dir));	//�ĸ������÷���
		xy = cT[i].Move();				//�����ƶ�̹��
		if (gAPI.GetRand() % 10 < 2) {	//���֮һ���ʿ���
			this->AddBullet(i + 1);
		}
		if (!CheckTank(xy, i + 1)) {	//���ؼ�ʱ,��������̹��
			CHARMAP tmap = {0};
			PM->SetMap(xy, tmap, true);
			this->SetTank(i);
		}
		PM->SetMap(cT[i].GetOldXY(), tmap, true);
		PV->PrintMap(cT[i]);
	}
	return false;
}

bool CCtrl::CheckTank(COORD& xy, byte tid)
{
	CHARMAP tmap;
	SHORT x, y; int power;
	byte bid = 0;
	for (x = -1; x < 2; x++)
	{
		for (y = -1; y < 2; y++)
		{
			tmap = map[xy.Y + y][xy.X + x];
			if (tmap.Bullet == true) {
				FindBullet({ xy.X + x,xy.Y + y }, &bid);
				if (tid < 3) {		//�ػ�����̹��
					tmap.TANKER = tid;
					PM->SetMap(xy, tmap, true);
					PV->PrintMap(cT[tid - 1]);
				}
				if (cT[tid - 1].GetTeam() ==
					cT[bid - 1].GetTeam())
					power = cT[tid - 1].GetBlood();
				else
					power = cT[tid - 1].GetBlood() -
					cT[bid - 1].GetPower();
				if (power > 0)
					cT[tid - 1].SetBlood(power);
				else {
					cT[tid - 1].SetBlood(0);
					cT[bid - 1].AddKills();
				}
				return power > 0;
			}
		}
	}
	return true;
}

bool CCtrl::FindBullet(COORD xy, byte* tid)
{
	COORD bxy = { 0,0 };
	auto begin = _bullets.begin();
	while (begin != _bullets.end()) {
		bxy = (*begin).GetOldXY();
		if (xy.X == bxy.X && xy.Y == bxy.Y) {
			if (tid == nullptr) {
				(*begin).SetBlood(0);
				return false;
			}
			*tid = (*begin).GetTid();
			map[xy.Y][xy.X].Bullet = false;
			PV->PrintPoint(xy, deStr�յ�);
			begin = _bullets.erase(begin);
			return true;
		}
		++begin;
	}
	++begin;		//���������
	return false;
}
#pragma endregion

//��ͼ����ͼ������
#pragma region ��ͼ��ͼ������
void CCtrl::PrintGInfo(unsigned int& msecond, WORD color)
{
	//	https://blog.csdn.net/xingcen/article/details/55669054
	if (++gGINFO.count % 10 < 5) return;//0.5��ˢ�½���
	else gGINFO.count = 0x00;			//����ʱ��
	char tick[MAX_PATH];				//���ڴ洢��ʽ��ʱ��
	time(&gGINFO.now);					//��ȡϵͳ���ں�ʱ��
	struct tm t;						//tm�ṹָ��
	localtime_s(&t, &gGINFO.now);		//��ȡ�������ں�ʱ��
	COORD xy = { MAP_W + 2,0 };			//��ʼ����ӡ����
	strftime(tick, MAX_PATH, "��ǰʱ�䣺%Y��%m��%d��%H:%M:%S", &t);
	PV->PrintPoint(xy, tick, color);	//��ӡ��ʽ����ʱ��
	xy.Y += 2;							//����
	unsigned short second = msecond / 1000;	//��¼��ǰ��
	sprintf_s(tick, "��Ϸʱ�䣺%02d:%02d:%03d",
		second / 60, second % 60, msecond % 1000);
	PV->PrintPoint(xy, tick, color);	//��ӡ��Ϸʱ��
	xy.Y += 4;
	byte i;
	for (i = 0; i < 5; i++, xy.Y++)		//ѭ����ʾ��
		PV->PrintPoint(xy, GAMEDesc[i], color);		//��ӡ��ʾ
	xy.Y++; int itmp[5] = { 0 }, itmp2[5] = { 0 };	//�����������
	cT[0].GetInfo(itmp);				//��ȡ���1
	if (gGINFO.player == 2)				//��ȡ���2
		cT[1].GetInfo(itmp2);
	for (i = 0; i < 5; i++, ++xy.Y)		//ѭ����ӡ�������
	{
		if (gGINFO.player == 1)			//�ж����1�����2
			sprintf_s(tick, PlayerInfo[i], itmp[i]);
		else
			sprintf_s(tick, PlayerInfo2[i], itmp[i], itmp2[i]);
		PV->PrintPoint(xy, tick, color);
	}
}

bool CCtrl::DrawMap()
{
	PV->PrintMap();
	system("PAUSE");
	return true;
}
#pragma endregion
