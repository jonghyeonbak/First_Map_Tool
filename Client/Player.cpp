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

// 깊이 우선 탐색, 너비 우선 탐색 = a스타 알고리즘이란 무엇인가

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

	// 이미지의 중심 좌표
	float	fCenterX = pTextureInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTextureInfo->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

	CDevice::GetInstance()->Get_Sprite()->Draw(pTextureInfo->pTexture,
		nullptr,	// 출력할 이미지 영역에 대한 렉트 구조체 주소값, null인 경우 이미지의 0,0을 기준으로 출력하게됨
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),	// 출력할 이미지의 중심 축 좌표를 vec3 타입의 주소값, null인 경우 0,0이 중심 좌표가 됨
		nullptr,	// 위치 좌표에 대한 vec3타입의 주소값, null인 경우 스크린 상의 0, 0 좌표 출력
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPlayer::Release(void)
{
	
}

void CPlayer::Move_AStar(void)
{
	list<TILE*>&	BestList = CAStarMgr::GetInstance()->GetBestList();

	// astar에 의한 길찾기가 성공했다면
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
