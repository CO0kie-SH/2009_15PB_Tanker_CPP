#include "CTanker.h"
CTanker::CTanker()
{
}

CTanker::~CTanker()
{
}

bool CTanker::TryMove(byte dir)
{
	if (dir != NULL && this->_dir != dir) {	//如果方向不同，则修改方向
		this->_dir = dir;
		return true;
	}
	COORD nxy = { this->_oxy.X,this->_oxy.Y };
	switch (this->_dir) {
	case 'W': ++nxy.Y; break;
	case 'A': --nxy.X; break;
	case 'S': --nxy.Y; break;
	case 'D': ++nxy.X; break;
	}
	if (nxy.Y == MAP_H) return false;
	this->_nxy = nxy;
	return true;
}

bool CTanker::Move()
{
	this->_oxy.X = this->_nxy.X;
	this->_oxy.Y = this->_nxy.Y;
	return true;
}
