#pragma once
//#include "CView.h"
#include "CData.h"

class CTanker
{
friend class CView;
public:
	CTanker();
	~CTanker();

	void init(COORD xy, int tid = 0) {
		this->_oxy.X = xy.X;
		this->_oxy.Y = xy.Y;
		this->_tid = tid;
	};
protected:
	COORD _oxy = { 0,0 };
	COORD _nxy = { 0,0 };
private:
	int _tid = 0;
};