#pragma once
#include <list>
using std::list;
#include "CView.h"
#include "CMap.h"
#include "CAStar.h"

class CCtrl
{
public:
	CCtrl(COORD xy);
	~CCtrl();

	/*��ʼ������*/
	bool InitCMD();

	/*��ʼ��Ϸ����*/
	int Go(int GameMode = 1, int Checkpoint = 1);

	/* �жϰ�������
	���ڰ����ж�	*/
	void GoKey(byte& key, unsigned int msecond);



private:	//	//	//���ں���
	/*	��ʼ��̹�˺���
	���ڳ�ʼ��̹��	*/
	void SetTank(int TankI);

	/*	�����ӵ�����
	���ڼ���ӵ��Ƿ��ܹ�����	*/
	bool AddBullet(byte tid);

	/*	�ӵ��ƶ�����
	�����ƶ��ӵ�	*/
	bool MoveBullet(CBullet& att, bool clean = false);

	/*	�ƶ�̹�˺���
	�����ƶ�ѭ��AI̹��	*/
	bool MoveTank(unsigned int& mSecond);

	/*	���̹����ײ����
	���ڼ��̹���Ƿ���ײ	*/
	bool CheckTank(COORD& xy, byte tid);

	/*	��Ϸ��Ϣ��ӡ����
	���ڵ�ͼ�Ҳ����Ϸ��Ϣ����	*/
	void PrintGInfo(unsigned int& msecond, WORD color = 0x0E);

	/*	���ӵ�����
	ͨ�����귵���ӵ��Ĺ���̹��	*/
	bool FindBullet(COORD xy, byte* tid);

	/*	�Զ����ͼ����
	���ڴ��ڻ���ͼ	*/
	bool DrawMap();

private:	//	//	//���ڱ���
	CTanker cT[7];	//̹������
	CView*	PV;		//��ͼ����
	CMap*	PM;		//��ͼ����
	CAStar	PA;		//Ѱ·�㷨
	COORD	_maxXY;	//�ڲ����ڴ�С
	byte	_tanks = 0x04;	//̹������
	list<CBullet> _bullets;	//�ӵ�����
};
