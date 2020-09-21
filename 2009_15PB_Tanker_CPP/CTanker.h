#pragma once
#include "CData.h"

class CTanker
{
friend class CView;
friend class CMap;
public:
	CTanker();
	~CTanker();

	void Init(COORD xy, int blood, int tid = 0, byte dir = 'W',
		byte team = 'A', WORD color = 0x0F) {
		this->_oxy.X = xy.X; this->_oxy.Y = xy.Y;
		this->_tid = tid;	 this->_dir = dir;
		this->_color = color;this->_blood = blood;
	};
	byte GetDir(bool to0123 = true) {
		if (to0123) {
			switch (this->_dir) {
			case 'W': return 0x00;
			case 'A': return 0x01;
			case 'D': return 0x02;
			case 'S': return 0x03; }
		}
		return this->_dir;
	};
	COORD GetOldXY() { return _oxy; };
	bool TryMove(byte dir = NULL);
	bool CheckMap();
	bool Move();
	bool IsAlive() { return this->_blood > 0; };
protected:
	COORD _oxy = { 0,0 };
	COORD _nxy = { 0,0 };
	byte _tid = 0;
	byte _dir = 'W';
	byte _team = 'A';
	WORD _color = 0x0F;
	int _blood = 0;
private:
	//int _id = 0;
};