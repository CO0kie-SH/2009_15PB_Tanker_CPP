#pragma once

#pragma comment(lib,"winmm.lib")	//�����������ӿ�
#include "CData.h"

class CBgm
{
public:
	/*	�������ܣ����ű�������*/
	void PlayMusic()
	{
		char temp[255];
		mciSendStringA(("open Data\\lxhzj.mp3 alias MUSIC"), temp, 255, 0);
		//mciSendStringA(("open Data\\bgm.wma alias MUSIC"), temp, 255, 0);
		mciSendStringA(("open Data\\Ak.mp3 alias MUSIC1"), temp, 255, 0);
		mciSendStringA(("open Data\\Death.mp3 alias MUSIC2"), temp, 255, 0);
		mciSendStringA(("open Data\\StartAttack.mp3 alias MUSIC3"), temp, 255, 0);
		mciSendStringA("play MUSIC repeat", NULL, 0, 0);
	};

	/*�������ܣ������ڵ�������Ч*/
	void PlayOp()
	{
		mciSendStringA("seek MUSIC1 to start", NULL, 0, 0);
		mciSendStringA("play MUSIC1", NULL, 0, 0);
	}

	/*�������ܣ�����̹��������Ч*/
	void PlayDeath()
	{
		mciSendStringA("seek MUSIC2 to start", NULL, 0, 0);
		mciSendStringA("play MUSIC2", NULL, 0, 0);
	}

	/*�������ܣ�����̹�˱�������Ч*/
	void PlayHit()
	{
		mciSendStringA("seek MUSIC3 to start", NULL, 0, 0);
		mciSendStringA("play MUSIC3", NULL, 0, 0);
	}
};

