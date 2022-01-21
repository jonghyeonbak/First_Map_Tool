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

	// ���� ������ Ÿ�� �ε����� ���Ѵ�.
	m_iStartIndex = GetTileIndex(vStart);

	int	iGoalIndex = GetTileIndex(vGoal);

	if (0 > m_iStartIndex || 0 > iGoalIndex)
		return;

	if (m_iStartIndex == iGoalIndex)
		return;

	// ��ǥ ������ ������ ���̸� astar Ž�� ����
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

		// �θ� Ÿ���� �������ϸ鼭 �տ������� bestlist ���� ä�� ������.
		m_BestList.push_front(vecTile[iRouteIndex]);
		iRouteIndex = vecTile[iRouteIndex]->iParentIndex;
	}
}


bool CAStarMgr::MakeRoute(int iStartIndex, int iGoalIndex)
{
	CObj*					pTerrain = CObjMgr::GetInstance()->Get_Terrain();
	vector<TILE*>&			vecTile  = dynamic_cast<CMyTerrain*>(pTerrain)->GetVecTile();
	vector<list<TILE*>>&	vecAdj   = dynamic_cast<CMyTerrain*>(pTerrain)->GetVecAdj();
	// ť�� �������� �����ϴ� �ʺ� �켱Ž���� Ư�� �� ���� ���� ���� ��带 ���� ����Ʈ�� ����

	if (!m_OpenList.empty())
		m_OpenList.pop_front();

	m_CloseList.push_back(iStartIndex); // ���������� ���������� �ƴϱ� ������ �ٷ� �˻���󿡼� ����

	for (auto& pTile : vecAdj[iStartIndex])
	{
		// ã�� ��ΰ� �� ������ �������� ���
		if (iGoalIndex == pTile->iIndex)
		{
			pTile->iParentIndex = iStartIndex;
			return true;
		}

		// ã�� ������ Ÿ���� close �Ǵ� open �ִ��� ����
		// ���� ������ �� ����Ʈ�� ��� ���ԵǾ� ���� �ʾ� �̵��� �� �ִ� ���ɼ��� ���� ��� ���� ����
		if (false == CheckClose(pTile->iIndex) &&
			false == CheckOpen(pTile->iIndex))
		{
			pTile->iParentIndex = iStartIndex;
			// ���� �ִ� �༮���� �Ǵ��߱� ������ open ��
			m_OpenList.push_back(pTile->iIndex);
		}
	}

	// �� �̻� �� �� �ִ� ���� ���ٴ� ��
	if (m_OpenList.empty())
		return false;

		//	Cost		PCost							GCost
		// �޸���ƽ = ���� ���� �������� ����(���� ��) + �� ���������� �Ÿ�
		// ��������
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

	// ���� �켱Ž��ó�� ���������� ã�� ������ ��� Ž��
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

	// 12�� ������ �������� ������ ��ġ �� ����
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

	// �� ���� ������ ���� ���͸� �������� �Ѵ�.
	// �������ʹ� �� ���Ϳ� ������ �̷�� ���� ���ͷ� ũ��� �ݵ�� 1�̾�� �Ѵ�.

	D3DXVECTOR3	vNormal[4] = {

		D3DXVECTOR3(-vDir[0].y, vDir[0].x, 0.f),
		D3DXVECTOR3(-vDir[1].y, vDir[1].x, 0.f),
		D3DXVECTOR3(-vDir[2].y, vDir[2].x, 0.f),
		D3DXVECTOR3(-vDir[3].y, vDir[3].x, 0.f)
	};

	// Ÿ���� �� ������ ���콺 ��ġ�� ���ϴ� ���� ���͸� ���Ѵ�.

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
		// ���Ⱑ ����� ���(����)
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
