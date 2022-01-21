#include "stdafx.h"
#include "Player.h"
#include "TextureMgr.h"
#include "Device.h"
#include "AStarMgr.h"
#include "TimeMgr.h"


CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	Release();
}

// ���� �켱 Ž��, �ʺ� �켱 Ž�� = a��Ÿ �˰����̶� �����ΰ�

HRESULT CPlayer::Initialize(void)
{
	m_tInfo.vPos = D3DXVECTOR3(400.f, 400.f, 0.f);
	m_wstrObjKey = L"Player";
	m_wstrStateKey = L"Dash";

	m_tFrame = FRAME(0.f, 10.f);

	return S_OK;
}

int CPlayer::Update(void)
{
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		D3DXVECTOR3		vMouse = GetMouse() - m_vScroll;
		CAStarMgr::GetInstance()->Start_AStar(m_tInfo.vPos, vMouse);
	}


	D3DXMATRIX		matScale, matTrans;
	
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, 
		m_tInfo.vPos.x + m_vScroll.x,
		m_tInfo.vPos.y + m_vScroll.y, 
		0.f);

	m_tInfo.matWorld = matScale * matTrans;

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update(void)
{
	CObj::Move_Frame();
	Move_AStar();

}

void CPlayer::Render(void)
{
	const	TEXINFO*		pTextureInfo = CTextureMgr::GetInstance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);
	if (nullptr == pTextureInfo)
		return;

	// �̹����� �߽� ��ǥ
	float	fCenterX = pTextureInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTextureInfo->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

	CDevice::GetInstance()->Get_Sprite()->Draw(pTextureInfo->pTexture,
		nullptr,	// ����� �̹��� ������ ���� ��Ʈ ����ü �ּҰ�, null�� ��� �̹����� 0,0�� �������� ����ϰԵ�
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),	// ����� �̹����� �߽� �� ��ǥ�� vec3 Ÿ���� �ּҰ�, null�� ��� 0,0�� �߽� ��ǥ�� ��
		nullptr,	// ��ġ ��ǥ�� ���� vec3Ÿ���� �ּҰ�, null�� ��� ��ũ�� ���� 0, 0 ��ǥ ���
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPlayer::Release(void)
{
	
}

void CPlayer::Move_AStar(void)
{
	list<TILE*>&	BestList = CAStarMgr::GetInstance()->GetBestList();

	// astar�� ���� ��ã�Ⱑ �����ߴٸ�
	if (!BestList.empty())
	{
		D3DXVECTOR3	vDir = BestList.front()->vPos - m_tInfo.vPos;

		float	fDistance = D3DXVec3Length(&vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		m_tInfo.vPos += vDir * 200* CTimeMgr::GetInstance()->Get_TimeDelta();
		
		if (3.f >= fDistance)
			BestList.pop_front();
	}

}
