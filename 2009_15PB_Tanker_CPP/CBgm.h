#pragma once

#pragma comment(lib,"winmm.lib")	//‘ÿ»Î“Ù¿÷¡¥Ω”ø‚
#include "CData.h"

class CBgm
{
public:
	void PlayMusic()
	{
		char temp[255];
		//mciSendStringA(("open Data\\lxhzj.mp3 alias MUSIC"), temp, 255, 0);
		mciSendStringA(("open Data\\bgm.wma alias MUSIC"), temp, 255, 0);
		//mciSendStringA(("open Data\\Ak.wav alias MUSIC1"), temp, 255, 0);
		//mciSendStringA(("open Data\\Death.wav alias MUSIC2"), temp, 255, 0);
		//mciSendStringA(("open Data\\Hit.wav alias MUSIC3"), temp, 255, 0);
		mciSendStringA("play MUSIC repeat", NULL, 0, 0);
	};
	void PlayOp()
	{
		mciSendStringA("seek MUSIC1 to start", NULL, 0, 0);
		mciSendStringA("play MUSIC1", NULL, 0, 0);
	}

	void PlayDeath()
	{
		mciSendStringA("seek MUSIC2 to start", NULL, 0, 0);
		mciSendStringA("play MUSIC2", NULL, 0, 0);
	}

	void PlayHit()
	{
		mciSendStringA("seek MUSIC3 to start", NULL, 0, 0);
		mciSendStringA("play MUSIC3", NULL, 0, 0);
	}
};

