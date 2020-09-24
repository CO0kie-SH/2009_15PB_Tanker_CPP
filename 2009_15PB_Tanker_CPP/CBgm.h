#pragma once

#pragma comment(lib,"winmm.lib")	//载入音乐链接库
#include "CData.h"

class CBgm
{
public:
	/*	函数功能：播放背景音乐*/
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

	/*函数功能：播放炮弹发射音效*/
	void PlayOp()
	{
		mciSendStringA("seek MUSIC1 to start", NULL, 0, 0);
		mciSendStringA("play MUSIC1", NULL, 0, 0);
	}

	/*函数功能：播放坦克死亡音效*/
	void PlayDeath()
	{
		mciSendStringA("seek MUSIC2 to start", NULL, 0, 0);
		mciSendStringA("play MUSIC2", NULL, 0, 0);
	}

	/*函数功能：播放坦克被击中音效*/
	void PlayHit()
	{
		mciSendStringA("seek MUSIC3 to start", NULL, 0, 0);
		mciSendStringA("play MUSIC3", NULL, 0, 0);
	}
};

