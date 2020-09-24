#include "CTanker.h"
CTanker::CTanker()
{
}

CTanker::~CTanker()
{
}

bool CTanker::TryMove(byte dir)
{
	switch (dir) {
	case 'I':dir = 'W'; break;
	case 'J':dir = 'A'; break;
	case 'K':dir = 'S'; break;
	case 'L':dir = 'D'; break;
	default:break; }
	if (dir != NULL && this->_dir != dir) {	//如果方向不同，则修改方向
		this->_dir = dir; this->_nxy = this->_oxy;
		return true;
	}
	return CheckMap();
}

bool CTanker::CheckMap()
{
	COORD nxy = { this->_oxy.X,this->_oxy.Y };
	switch (this->_dir) {
	case 'W': --nxy.Y; break;
	case 'A': --nxy.X; break;
	case 'S': ++nxy.Y; break;
	case 'D': ++nxy.X; break;
	}
	if (nxy.Y == MAP_H - 1) return false;
	else if (nxy.Y == 0) return false;
	if (nxy.X == MAP_W - 1) return false;
	else if (nxy.X == 0) return false;

	CHARMAP tmap; SHORT i, j;
	for (i = -1; i < 2; i++)
	{
		for (j = -1; j < 2; j++)
		{
			tmap = map[nxy.Y + j][nxy.X + i];
			if (tmap.Wall == true)
				return false;
			else if (tmap.Earth == true)
				return false;
			else if (tmap.Rever == true)
				return false;
			else if (tmap.TANKER != 0x00)
				return false;
		}
	}
	this->_nxy = nxy;
	return true;
}

/*移动坦克函数*/
COORD CTanker::Move()
{
	this->_oxy = { _nxy.X,_nxy.Y };
	return this->_oxy;
}
