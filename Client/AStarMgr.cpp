#include "stdafx.h"
#include "AStarMgr.h"
#include "ObjMgr.h"
#include "MyTerrain.h"

IMPLEMENT_SINGLETON(CAStarMgr)

CAStarMgr::CAStarMgr()
{
}


CAStarMgr::~CAStarMgr()
{
	Release();
}

void CAStarMgr::Start_AStar(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal)
{
	m_OpenList.clear();
	m_CloseList.clear();
	m_BestList.clear();

	CObj*					pTerrain = CObjMgr::GetInstance()->Get_Terrain();
	vector<TILE*>&			vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetVecTile();

	// 시작 지점의 타일 인덱스를 구한다.
	m_iStartIndex = GetTileIndex(vStart);

	int	iGoalIndex = GetTileIndex(vGoal);

	if (0 > m_iStartIndex || 0 > iGoalIndex)
		return;

	if (m_iStartIndex == iGoalIndex)
		return;

	// 목표 지점이 못가는 곳이면 astar 탐색 중지
	if (1 == vecTile[iGoalIndex]->byOption)
		return;

	if (true == MakeRoute(m_iStartIndex, iGoalIndex))
		MakeBestList(iGoalIndex);

}


void CAStarMgr::Release(void)
{
	m_OpenList.clear();
	m_CloseList.clear();
	m_BestList.clear();
}

void CAStarMgr::MakeBestList(int iGoalIndex)
{
	CObj*					pTerrain = CObjMgr::GetInstance()->Get_Terrain();
	vector<TILE*>&			vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetVecTile();

	m_BestList.push_front(vecTile[iGoalIndex]);

	int	iRouteIndex = vecTile[iGoalIndex]->iParentIndex;

	while (true)
	{
		if (iRouteIndex == m_iStartIndex)
			break;

		// 부모 타일을 역추적하면서 앞에서부터 bestlist 값을 채워 나간다.
		m_BestList.push_front(vecTile[iRouteIndex]);
		iRouteIndex = vecTile[iRouteIndex]->iParentIndex;
	}
}


bool CAStarMgr::MakeRoute(int iStartIndex, int iGoalIndex)
{
	CObj*					pTerrain = CObjMgr::GetInstance()->Get_Terrain();
	vector<TILE*>&			vecTile  = dynamic_cast<CMyTerrain*>(pTerrain)->GetVecTile();
	vector<list<TILE*>>&	vecAdj   = dynamic_cast<CMyTerrain*>(pTerrain)->GetVecAdj();
	// 큐의 형식으로 동작하는 너비 우선탐색의 특성 상 가장 먼저 들어온 노드를 오픈 리스트에 제거

	if (!m_OpenList.empty())
		m_OpenList.pop_front();

	m_CloseList.push_back(iStartIndex); // 시작지점은 도착지점이 아니기 때문에 바로 검색대상에서 제거

	for (auto& pTile : vecAdj[iStartIndex])
	{
		// 찾은 경로가 골 지점에 도달했을 경우
		if (iGoalIndex == pTile->iIndex)
		{
			pTile->iParentIndex = iStartIndex;
			return true;
		}

		// 찾은 인접한 타일이 close 또는 open 있는지 조사
		// 현재 조건은 두 리스트에 모두 포함되어 있지 않아 이동할 수 있는 가능성을 가진 노드 임을 조사
		if (false == CheckClose(pTile->iIndex) &&
			false == CheckOpen(pTile->iIndex))
		{
			pTile->iParentIndex = iStartIndex;
			// 갈수 있는 녀석으로 판단했기 때문에 open 편성
			m_OpenList.push_back(pTile->iIndex);
		}
	}

	// 더 이상 갈 수 있는 길이 없다는 것
	if (m_OpenList.empty())
		return false;

		//	Cost		PCost							GCost
		// 휴리스틱 = 최초 시작 시점과의 길이(깊이 값) + 골 지점까지의 거리
		// 오름차순
	int	iOriginStart = m_iStartIndex;

	m_OpenList.sort([&](int iDest, int iSour)
	{
		D3DXVECTOR3 vPCost1 = vecTile[iDest]->vPos - vecTile[m_iStartIndex]->vPos;
		D3DXVECTOR3 vGCost1 = vecTile[iDest]->vPos - vecTile[iGoalIndex]->vPos;
		float fCost1 = D3DXVec3Length(&vPCost1) + D3DXVec3Length(&vGCost1);

		D3DXVECTOR3 vPCost2 = vecTile[iSour]->vPos - vecTile[m_iStartIndex]->vPos;
		D3DXVECTOR3 vGCost2 = vecTile[iSour]->vPos - vecTile[iGoalIndex]->vPos;
		float fCost2 = D3DXVec3Length(&vPCost2) + D3DXVec3Length(&vGCost2);

		return fCost1 < fCost2;
	});	

	// 깊이 우선탐색처럼 도착지점을 찾을 때까지 계속 탐색
	return MakeRoute(m_OpenList.front(), iGoalIndex);
}


int CAStarMgr::GetTileIndex(const D3DXVECTOR3& vPos)
{
	CObj* pTerrain = CObjMgr::GetInstance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetVecTile();

	if (vecTile.empty())
		return -1;

	for (size_t iIndex = 0; iIndex < vecTile.size(); ++iIndex)
	{
		if (Picking(vPos, iIndex))
		{
			return iIndex;
		}
	}

	return -1;
}

bool CAStarMgr::Picking(const D3DXVECTOR3& vPos, int iIndex)
{
	CObj* pTerrain = CObjMgr::GetInstance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->GetVecTile();

	if (vecTile.empty())
		return false;

	// 12시 방향을 기준으로 점들의 위치 값 저장
	D3DXVECTOR3		vPoint[4] =
	{
		D3DXVECTOR3(vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f),
		D3DXVECTOR3(vecTile[iIndex]->vPos.x + (TILECX / 2.f), vecTile[iIndex]->vPos.y, 0.f),
		D3DXVECTOR3(vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f),
		D3DXVECTOR3(vecTile[iIndex]->vPos.x - (TILECX / 2.f), vecTile[iIndex]->vPos.y, 0.f),
	};

	D3DXVECTOR3		vDir[4] = {

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	// 각 방향 벡터의 법선 벡터를 만들어줘야 한다.
	// 법선벡터는 각 벡터와 직각을 이루는 방향 벡터로 크기는 반드시 1이어야 한다.

	D3DXVECTOR3	vNormal[4] = {

		D3DXVECTOR3(-vDir[0].y, vDir[0].x, 0.f),
		D3DXVECTOR3(-vDir[1].y, vDir[1].x, 0.f),
		D3DXVECTOR3(-vDir[2].y, vDir[2].x, 0.f),
		D3DXVECTOR3(-vDir[3].y, vDir[3].x, 0.f)
	};

	// 타일의 네 점에서 마우스 위치를 향하는 방향 벡터를 구한다.

	D3DXVECTOR3	vMouseDir[4] = {

		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};
	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		// 기울기가 양수인 경우(예각)
		if (0.f < D3DXVec3Dot(&vMouseDir[i], &vNormal[i]))
			return false;
	}

	return true;
}

bool CAStarMgr::CheckClose(int iIndex)
{
	for (int& iCloseIndex : m_CloseList)
	{
		if (iIndex == iCloseIndex)
			return true;
	}

	return false;
}


bool CAStarMgr::CheckOpen(int iIndex)
{
	for (int& iOpenIndex : m_OpenList)
	{
		if (iIndex == iOpenIndex)
			return true;
	}

	return false;
}
