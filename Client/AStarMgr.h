#pragma once

#include "Include.h"
#include "MyTerrain.h"

class CAStarMgr
{
	DECLARE_SINGLETON(CAStarMgr)

private:
	CAStarMgr();
	~CAStarMgr();

public:
	list<TILE*>&		GetBestList(void) { return m_BestList; }

public:
	void		Start_AStar(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal);
	void		Release(void);

private:
	bool		MakeRoute(int iStartIndex, int iGoalIndex);
	void		MakeBestList(int iGoalIndex);
	int			GetTileIndex(const D3DXVECTOR3& vPos);
	bool		Picking(const D3DXVECTOR3& vPos, int iIndex);
	bool		CheckClose(int iIndex);
	bool		CheckOpen(int iIndex);

private:
	list<int>			m_OpenList;
	list<int>			m_CloseList;
	list<TILE*>			m_BestList;

	int					m_iStartIndex;

};

