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
	gOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);//�õ�������
	gAPI.OutputDebugPrintf("��ʼ����");
}

CCtrl::~CCtrl()
{
	delete PV;				//�ͷ���ͼ��&��ͼ��
	delete PM;
	//CloseHandle(gOUTPUT);	//�ͷ�������
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
				while (Go(menu, gGINFO.levels)) {
					++gGINFO.levels;
				}
			else if (4 == menu)
				Go(de�浵��Ϸ);
			else {
				if (this->DrawMap())
					Go(de�Զ����ͼ);
				PV->PrintMap();
				gGINFO.menu = 0x00;
			}
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
			4, 'S', 'B', deCol�з�������, 500, 44);
	else if (0 == TankI) {
		this->cT[0].Init({ MAP_W / 2, MAP_H - 2 }, 100,
			1, 'D', 'A', 0x04);
		tmap.TANKER = TankI + 1;
		PM->SetMap(cT[TankI].GetOldXY(), tmap, true);
		PV->PrintMap(cT[TankI]);
	}
	else if (1 == TankI) {
		this->cT[1].Init({ MAP_W / 2 + 4, MAP_H - 2 }, 100,
			2, 'W', 'A', 0x09);
		tmap.TANKER = TankI + 1;
		PM->SetMap(cT[TankI].GetOldXY(), tmap, true);
		PV->PrintMap(cT[TankI]);
	}
}

int CCtrl::Go(int GameMode, int Checkpoint)
{
	byte bGame = 'G', key = 0, i = 0;
	COORD xy = { 0,0 }; CHARMAP tmap = { 0 };
	unsigned int msecond = 0; int nums = 0;
	this->_tanks = 4;
	if (de������Ϸ���� == GameMode) {		//��ʼ��������Ϸ
		if (!PM->SetMap(Checkpoint)) return 0;
		PV->PrintMap();
		gGINFO.player = 1; this->SetTank(0);
		this->cT[1].Init({ 1, 1 }, 0, 2);
	}
	else if (de˫����Ϸ���� == GameMode) {	//��ʼ��˫����Ϸ
		if (!PM->SetMap(Checkpoint)) return 0;
		PV->PrintMap(); gGINFO.player = 2;
		this->SetTank(0); this->SetTank(1);
	}
	else if (de�Զ����ͼ == GameMode) {	//��ʼ���Զ�����Ϸ
		gGINFO.player = 1; this->SetTank(0);
		this->cT[1].Init({ 1, 1 }, 0, 2);
	}
	this->SetTank(2); this->SetTank(3);
	if (de�浵��Ϸ == GameMode) {		//��ʼ���浵
		this->PV->ReadGame(cT);
		this->PV->PrintMap();
		gGINFO.player = cT[1].IsAlive() ? 2 : 1;
		tmap = { 0 };
		for (i = 0; i < _tanks; i++)
		{
			PM->SetMap(cT[i].GetOldXY(), tmap, true);
			cT[i].mSecond = 0;
			this->PV->PrintMap(cT[i]);
		}
		Checkpoint = gGINFO.levels;
	}
	gGINFO.count = 7;
#pragma endregion

//��ʼѭ����Ϸ
#pragma region ��ʼѭ����Ϸ
	do {
		msecond = (unsigned int)(
			GetTickCount64() - gGINFO.start);
		PrintGInfo(msecond);
#pragma region AI�����߼���
		tmap.TANKER = 0;						//���õ�ͼ̹��false
		for (i = 2; i < this->_tanks; i++) {	//ѭ������
			PM->SetMap(cT[i].GetOldXY(), tmap, true);
			PV->PrintMap(cT[i], true);			//����̹��
		}
		auto begin = _bullets.begin();			//�ӵ�����
		while (begin != _bullets.end()) {		//ѭ���ӵ�
			if (MoveBullet(*begin, true)) ++begin;	//�ƶ�
			else begin = _bullets.erase(begin);	//���ʧ����ɾ��
		}
		MoveTank(msecond);						//�����ƶ�̹�˺���
		begin = _bullets.begin();				//ѭ����ʼ
		while (begin != _bullets.end()) {
			if ((*begin).IsAlive()) ++begin;
			else begin = _bullets.erase(begin);
		}
#pragma endregion
#pragma region �˹������߼���
		this->GoKey(key, msecond);
		if (key == KEY_ESC)	bGame = NULL;
		for (i = 0,nums=0; i < gGINFO.player; i++)
		{
			xy = cT[i].GetOldXY();
			nums += cT[i].GetInfo2();
			this->CheckTank(xy, i + 1);
			if (!cT[i].IsAlive()) {
				bGame = NULL; break;
			}
		}
#pragma endregion
#pragma region ��β���ж�
		if (de�浵��Ϸ != GameMode && nums > 99) bGame = NULL;
		if (key == 'P') {
			gGINFO.start = (unsigned int)(
				GetTickCount64() - gGINFO.start);		//�洢��Ϸ��ʱ
			msecond = 0x02; gGINFO.count = 7;			//��ʶ��Ϸ��ͣ
			this->PrintGInfo(msecond, deCol��ǽ);		//��ӡ��������
			while (i = _getch()) {
				if (i == 'N' || i == 'n') {
					gGINFO.start = (unsigned int)
						(GetTickCount64() - gGINFO.start);	//ת��ʱ��
					break;
				}
				else if (i == 'Y' || i == 'y') {
					gGINFO.start = (unsigned int)
						(GetTickCount64() - gGINFO.start);	//ת��ʱ��
					this->PV->SaveGame(cT);
					break;
				}
			}
			msecond = 0x03; gGINFO.count = 7;			//��ʶ��Ϸ��ͣ
			this->PrintGInfo(msecond);			//��ӡ��������
		}
		if (bGame) Sleep(78);
#pragma endregion
	} while (bGame);
#pragma region ������Ϸ����
	gGINFO.count = 7; PrintGInfo(msecond, 0x0C);
	PV->PrintPoint({ MAP_W / 2 - 6,MAP_H / 2 },
		"��Ϸ��������Y�˳���", 0x0C);
	if (nums > 99 && de�浵��Ϸ != GameMode)
		printf_s("����������һ��%d", Checkpoint + 1);
	else if (key != KEY_ESC) {
		for (i = 0; i < gGINFO.player; i++)
			if (!cT[i].IsAlive())
				printf_s("���%d������", i + 1);
	}
	while (true) {
		if ((i = _getch()) && KEY_DOWN('Y'))
			if (i == 'Y' || i == 'y')
				break;
		Sleep(11);
	}
	memset(map, 0, MAP_H * MAP_W);			//������Ϸ������ʼ����ͼ
	PV->PrintMap(); gGINFO.menu = 0x00;		//��ʼ������
	this->_bullets.clear();					//����ӵ�����
	if (nums > 99) return 1;
#pragma endregion
	return 0;
}

void CCtrl::GoKey(byte& key, unsigned int msecond)
{
	CHARMAP tmap = { 0 };					//��ʼ����ͼ����
	switch (key = gAPI.GetKey()) {			//ͨ��API���ؼ�ֵ
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
		}
		else {
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
	//case KEY_ESC: bGame = 0x00; break;
	default: break;
	}
}

#pragma endregion


//̹�ˡ��ӵ�������
#pragma region ̹���ӵ�������

/*	�����ӵ�����
	���ڼ���ӵ��Ƿ��ܹ�����	*/
bool CCtrl::AddBullet(byte tid)
{
	CTanker& tank = this->cT[tid-1];		//����̹��
	CBullet att;							//�����ӵ�
	COORD xy = tank.GetOldXY();				//��ȡ̹������
	if (map[xy.Y][xy.X].Bullet == true)		//�������Ϊ�ӵ����򷵻�
		return false;						//��������
	if(att.Init(xy, tank.GetDir(false),tank.GetTid())){
		if (MoveBullet(att)) {				//��ʼ���ӵ��󣬳����ƶ�
			_bullets.push_back(att);		//����ɹ�����������
			return true;					//��������
		}
	}
	return false;
}

/*	�ӵ��ƶ�����
	�����ƶ��ӵ�	*/
bool CCtrl::MoveBullet(CBullet& att, bool clean)
{
	if (!att.IsAlive()) return false;			//��������������򷵻�
	COORD xy = att.GetOldXY();					//��ȡ�ӵ�����
	CHARMAP tmap = map[xy.Y][xy.X];				//��ȡ�����ͼ��
	if (clean) {								//�ж��ػ�����
		map[xy.Y][xy.X].Bullet = false;			//������뻹ԭ�ӵ������ػ�
		if (tmap.Rever == true)					//��ԭ��ɫ����
			PV->PrintPoint(xy, deStr����, deCol����);
		else if (tmap.Grass == true)			//��ԭ��ɫ��ƺ
			PV->PrintPoint(xy, deStr��ƺ, deCol��ƺ);
		else PV->PrintPoint(xy, deStr�յ�);		//��ԭ�յ�
	}
	if (att.TryMove()) {						//��ͼ�ƶ��ӵ�
		xy = att.Move();						//����ɹ������ƶ�
		tmap = map[xy.Y][xy.X];					//��ˢ�µ�ͼ
		if (tmap.Rever == true)					//��������ɫ���ӵ�
			PV->PrintPoint(xy, deStr�ӵ�, deCol����);
		else if (tmap.Grass == true) {}			//��ƺ���ػ�
		else if (tmap.Earth == true) {			//������ǽ������
			att.SetBlood(0);					//�����ӵ�
			tmap = { 0 };
			PM->SetMap(xy, tmap);				//���õ�ͼ
			PV->PrintPoint(xy, deStr�յ�);		//���յ�
			return false;						//��������
		}
		else if(tmap.Bullet == true){			//�ӵ����ӵ���ײ
			FindBullet(xy, nullptr);			//�ҵ��ӵ��㲢����
			att.SetBlood(0);					//��������
			map[xy.Y][xy.X].Bullet = false;		//���õ�ͼ
			PV->PrintPoint(xy, deStr�յ�);		//���յ�
			return	false;						//��������
		} else {								//����
			map[xy.Y][xy.X].Bullet = true;		//���õ�ͼ
			PV->PrintPoint(xy, deStr�ӵ�);		//���ӵ�
		}
	}
	else return false;		//�����ͼ�ƶ�ʧ�ܣ��򷵻ؼ�
	return true;
}

/*	�ƶ�̹�˺���
	�����ƶ�ѭ��AI̹��	*/
bool CCtrl::MoveTank(unsigned int& mSecond)
{
	CHARMAP tmap = { 0 };						//�������
	COORD xy = { 0,0 };							//�������
	byte dir, i, isMove = NULL;					//�������
	for (i = 2; i < this->_tanks; i++) {		//ѭ��̹��
		if (!cT[i].IsAlive()) continue;			//���������������
		tmap.TANKER = i + 1;					//����ͼ����Ϊ̹��
		xy = cT[i].GetOldXY();					//��ȡ̹������

		//���	̹���ٶ�<(�ѹ�ȥ��ʱ��) (��Ϸʱ��-̹���ϴ�ʱ��)���ƶ�
		if (cT[i].GetSpeed() < (int)(mSecond - cT[i].mSecond)) {
			cT[i].mSecond = mSecond;			//����̹��ʱ��
			do {								//ѭ���ƶ�̹��
				gAPI.RandDir(dir);				//ͨ��API��1/24��%
			} while (!cT[i].TryMove(dir));		//�ĸ������÷���
			xy = cT[i].Move();					//�����ƶ�̹��
			if (gAPI.GetRand() % 10 < 2) {		//���֮һ���ʿ���
				this->AddBullet(i + 1);			//�����ӵ�
			}	isMove = true;					//�����ƶ��ɹ�
		}
		if (!CheckTank(xy, i + 1)) {			//���ؼ�ʱ,��������̹��
			tmap = { 0 };
			PM->SetMap(xy, tmap, true);			//���õ�ͼ
			this->SetTank(i);					//����
		}
		PM->SetMap(cT[i].GetOldXY(), tmap, true);	//���õ�ͼ
		PV->PrintMap(cT[i]);						//��ӡ̹��
	}
	return isMove;
}

/*	���̹����ײ����
	���ڼ��̹���Ƿ���ײ	*/
bool CCtrl::CheckTank(COORD& xy, byte tid)
{
	CHARMAP tmap;								//�������
	SHORT x, y; int power;						//�������
	byte bid = 0;								//�������
	for (x = -1; x < 2; x++) {					//ѭ��̹����x
		for (y = -1; y < 2; y++) {				//ѭ��̹����y
			tmap = map[xy.Y + y][xy.X + x];		//��ȡ�õ�����
			if (tmap.Bullet == true) {			//����õ�Ϊ�ӵ�
				FindBullet({ xy.X + x,xy.Y + y }, &bid);//�����ҳ�̹��
				if (tid < 3) {					//����������̹��
					tmap.TANKER = tid;			//���»���
					PM->SetMap(xy, tmap, true);	//���õ�ͼ
					PV->PrintMap(cT[tid - 1]);	//��ӡ̹��
				}
				if (cT[tid - 1].GetTeam() ==	//��ȡ�ӵ�ǿ��
					cT[bid - 1].GetTeam())		//��ȡ�ӵ�ǿ��
					power = cT[tid - 1].GetBlood();//��ȡ�ӵ�ǿ��
				else
					power = cT[tid - 1].GetBlood() -
					cT[bid - 1].GetPower();		//��ȡ�ӵ�ǿ��
				if (power > 0)					//��������Ѫ��>0
					cT[tid - 1].SetBlood(power);//����Ѫ��
				else {							//����
					cT[tid - 1].SetBlood(0);	//����Ѫ��0
					cT[bid - 1].AddKills();		//����ɱ����
					gGINFO.count = 7;
				}
				return power > 0;				//�����Ƿ���
			}
		}
	}
	return true;
}

/*	���ӵ�����
	ͨ�����귵���ӵ��Ĺ���̹��	*/
bool CCtrl::FindBullet(COORD xy, byte* tid)
{
	COORD bxy = { 0,0 };						//�������
	auto begin = _bullets.begin();				//�������
	while (begin != _bullets.end()) {			//ѭ���ӵ�����
		bxy = (*begin).GetOldXY();				//��ȡ�ӵ�����
		if (xy.X == bxy.X && xy.Y == bxy.Y) {	//�ж�����
			if (tid == nullptr) {				//���ӵ���ײ����
				(*begin).SetBlood(0);			//�������ӵ�
				return true;					//���غ���
			}
			*tid = (*begin).GetTid();			//�����ӵ�����
			map[xy.Y][xy.X].Bullet = false;		//���õ�ͼΪ��
			PV->PrintPoint(xy, deStr�յ�);		//��ӡ�յ�
			(*begin).SetBlood(0);				//����ӵ�
			return true;						//���غ���
		}
		++begin;								//ѭ������
	}
	return false;
}

#pragma endregion

//��ͼ����ͼ������
#pragma region ��ͼ��ͼ������

/*	��Ϸ��Ϣ��ӡ����
	���ڵ�ͼ�Ҳ����Ϸ��Ϣ����	*/
void CCtrl::PrintGInfo(unsigned int& msecond, WORD color)
{
	//�ο� https://blog.csdn.net/xingcen/article/details/55669054
	if (++gGINFO.count % 10 < 7) return;	//0.5��ˢ�½���
	else gGINFO.count = 0x00;				//����ʱ��
	char tick[MAX_PATH], i;					//���ڴ洢��ʽ��ʱ��
	time(&gGINFO.now);						//��ȡϵͳ���ں�ʱ��
	struct tm t;							//tm�ṹָ��
	localtime_s(&t, &gGINFO.now);			//��ȡ�������ں�ʱ��
	COORD xy = { MAP_W + 2,0 };				//��ʼ����ӡ����
	strftime(tick, MAX_PATH, "��ǰʱ�䣺%Y��%m��%d��%H:%M:%S", &t);
	PV->PrintPoint(xy, tick, color);		//��ӡ��ʽ����ʱ��
	xy.Y += 2;								//����
	unsigned short second = msecond / 1000;		//��¼��ǰ��
	sprintf_s(tick, "��Ϸʱ�䣺%02d:%02d:%03d",	//��ʽ���ַ���
		second / 60, second % 60, msecond % 1000);
	PV->PrintPoint(xy, tick, color);		//��ӡ��Ϸʱ��
	xy.Y += 4;
	if (0x01 == msecond) {					//������Զ����ͼ
		for (i = 0; i < 7; i++, ++xy.Y)		//ѭ���������ģʽ
			PV->PrintPoint(xy, szKEYs[
				color == 0xFF ? 5 : i], MColor[i]);
		return;
	}
	else if (0x02 == msecond) {
		for (i = 0; i < 5; i++, ++xy.Y)		//ѭ���������ģʽ
			PV->PrintPoint(xy, cstr��ͣ[i], color);
		return;
	}
	else if (0x03 == msecond) {
		for (i = 0; i < 6; i++, ++xy.Y)		//ѭ���������ģʽ
			PV->PrintPoint(xy, cstr��ͣ[4], color);
		return;
	}
	for (i = 0; i < 5; i++, xy.Y++)			//ѭ��������ʾ
		PV->PrintPoint(xy, GAMEDesc[i], color);		//��ӡ��ʾ
	xy.Y++; int itmp[5] = { 0 }, itmp2[5] = { 0 };	//�����������
	cT[0].GetInfo(itmp);					//��ȡ���1
	if (gGINFO.player == 2)					//�ж��������
		cT[1].GetInfo(itmp2);				//��ȡ���2��Ϣ
	for (i = 0; i < 6; i++, ++xy.Y) {		//ѭ����ӡ�������
		if (gGINFO.player == 1)				//�ж����1�����2
			sprintf_s(tick, PlayerInfo[i], itmp[i]);
		else
			sprintf_s(tick, PlayerInfo2[i], itmp[i], itmp2[i]);
		PV->PrintPoint(xy, tick, color);	//��ӡ�����Ϣ
	}
}

/*	�Զ����ͼ����
	���ڴ��ڻ���ͼ	*/
bool CCtrl::DrawMap()
{
	PV->PrintMap();									//ˢ�½���
	INPUT_RECORD ir = {};							//�������
	DWORD num = 0;									//�������
	COORD pos;										//�������
	CHARMAP tmap;									//�������
	byte key;										//�������
	unsigned int msecond = 0x01;					//�������
	memset(map, 0, MAP_H * MAP_W);					//���õ�ͼ
	gGINFO.count = 7; PrintGInfo(msecond);			//��ʽ����Ļ����
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);	//��ȡ����̨������
	SetConsoleMode(hInput,
		ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);	//���ÿ���̨ģʽ����������¼�
	while ((key = gAPI.GetKey()) != 'J') {			//��ȡ����
		if (key == 'H') break; Sleep(10);			//������ʵ��ӳ�
		ReadConsoleInput(hInput, &ir, 1, &num);		//��ȡ������
		if (ir.EventType == MOUSE_EVENT) {			//�ж�����¼�
			pos = ir.Event.MouseEvent.dwMousePosition;	//��ȡ�������λ��
			if (pos.X<2 || pos.Y>MAP_H - 3)	continue;	//�жϱ߽�
			pos.X = pos.X / 2 - 1; --pos.Y;			//ת����ͼ����
			if (pos.X >= MAP_W) continue;			//�жϳ����߽�
			else if (pos.Y < 3) continue;			//�жϳ����߽�
			tmap = { 0 };							//��ʼ����ͼ��
			if ('D' == key) tmap = { 0 };			//�жϰ������յ�
			else if ('W' == key) tmap.Wall = true;	//����ǽ
			else if ('F' == key) tmap.Earth = true;	//����ǽ
			else if ('G' == key) tmap.Grass = true;	//����ƺ
			else if ('R' == key) tmap.Rever = true;	//������
			else continue;							//���û�а�����������
			map[pos.Y][pos.X] = tmap;				//���õ�ͼ
			pos.X = pos.X * 2 + 2; pos.Y++;			//ת���������꣬������
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
			PV->PrintMap(tmap);						//��ӡ��ͼ��
		}
	}
	gGINFO.count = 7; PrintGInfo(msecond, 0xFF);	//�ػ�������Ϣ
	return 'J' == key;								//���ز�������
}
#pragma endregion
