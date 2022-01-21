#include "stdafx.h"
#include "MyTerrain.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"


CMyTerrain::CMyTerrain()
{
}


CMyTerrain::~CMyTerrain()
{
	Release();
}

HRESULT CMyTerrain::Initialize(void)
{
	if (FAILED(LoadTile(L"../Data/Map.dat")))
		return E_FAIL;

	ReadyAdjacency();

	return S_OK;
}

int CMyTerrain::Update(void)
{
	D3DXVECTOR3	vMouse = GetMouse();

	if (0.f > vMouse.x)
		m_vScroll.x += 300.f *  CTimeMgr::GetInstance()->Get_TimeDelta();

	if (WINCX < vMouse.x)
		m_vScroll.x -= 300.f *  CTimeMgr::GetInstance()->Get_TimeDelta();

	if (0.f > vMouse.y)
		m_vScroll.y += 300.f *  CTimeMgr::GetInstance()->Get_TimeDelta();

	if (WINCY < vMouse.y)
		m_vScroll.y -= 300.f *  CTimeMgr::GetInstance()->Get_TimeDelta();


	return OBJ_NOEVENT;
}

void CMyTerrain::Late_Update(void)
{

}

void CMyTerrain::Render(void)
{
	D3DXMATRIX	matWorld, matScale, matTrans;
	TCHAR		szBuf[64] = L"";
	int			iIndex = 0;

	int			iCullX = int(-m_vScroll.x) / TILECX;
	int			iCullY = int(-m_vScroll.y) / (TILECY / 2);

	int			iCullEndX = WINCX / TILECX;
	int			iCullEndY = WINCY / (TILECY / 2);


	for (int i = iCullY; i < iCullY + iCullEndY + 2; ++i)
	{
		for (int j = iCullX; j < iCullX + iCullEndX + 2; ++j)
		{
			int	iIndex = i * TILEX + j;

			if(0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			D3DXMatrixTranslation(&matTrans,
				m_vecTile[iIndex]->vPos.x + m_vScroll.x,
				m_vecTile[iIndex]->vPos.y + m_vScroll.y,
				m_vecTile[iIndex]->vPos.z);

			matWorld = matScale * matTrans;

			const	TEXINFO*		pTextureInfo = CTextureMgr::GetInstance()->Get_Texture(L"Terrain", L"Tile", m_vecTile[iIndex]->byTileIdx);
			if (nullptr == pTextureInfo)
				return;

			// 이미지의 중심 좌표
			float	fCenterX = pTextureInfo->tImgInfo.Width / 2.f;
			float	fCenterY = pTextureInfo->tImgInfo.Height / 2.f;

			CDevice::GetInstance()->Get_Sprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->Get_Sprite()->Draw(pTextureInfo->pTexture,
				nullptr,	// 출력할 이미지 영역에 대한 렉트 구조체 주소값, null인 경우 이미지의 0,0을 기준으로 출력하게됨
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f),	// 출력할 이미지의 중심 축 좌표를 vec3 타입의 주소값, null인 경우 0,0이 중심 좌표가 됨
				nullptr,	// 위치 좌표에 대한 vec3타입의 주소값, null인 경우 스크린 상의 0, 0 좌표 출력
				D3DCOLOR_ARGB(255, 255, 255, 255));

			swprintf_s(szBuf, L"%d", iIndex);

			CDevice::GetInstance()->Get_Font()->DrawTextW(CDevice::GetInstance()->Get_Sprite(),
				szBuf,
				lstrlen(szBuf),
				nullptr,
				0,
				D3DCOLOR_ARGB(255, 255, 0, 0));

			++iIndex;
		}
	}
}

void CMyTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<TILE*>);
	m_vecTile.clear();
}

HRESULT CMyTerrain::LoadTile(const TCHAR* pTilePath)
{
	HANDLE hFile = CreateFile(pTilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	TILE*	pTile = nullptr;
	
	while (true)
	{
		pTile = new TILE;

		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

		if (0 == dwByte)
		{
			Safe_Delete(pTile);
			break;
		}

		m_vecTile.push_back(pTile);
	}
	CloseHandle(hFile);

	return S_OK;
}

void CMyTerrain::ReadyAdjacency(void)
{
	m_vecAdjacency.resize(m_vecTile.size());

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int		iIndex = i * TILEX + j;

			// 왼쪽 상단 대각선
			// 갈 수 있는 옵션(0)인지 조사

			if (0 != i && (0 != iIndex % (TILEX * 2)))
			{
				 //홀수 줄은 20씩 감소 짝수 줄은 21씩 감소
				 if  ((0 != i % 2)   // 홀수 줄인지 판정
				 &&  (!m_vecTile[iIndex - TILEX]->byOption)) // 홀수 줄의 옵션이 0인지 판정
				 {
					 m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - TILEX]);
				 }
				 else if ((0 == i % 2)   // 짝수 줄인지 판정
					 && (!m_vecTile[iIndex - (TILEX + 1)]->byOption))
				 {
					 m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - (TILEX + 1)]);
				 }
			}

			// 오른쪽 상단 대각선
			// 갈 수 있는 옵션(0)인지 조사

			if (0 != i && ((TILEX * 2 - 1) != iIndex % (TILEX * 2)))
			{
				//홀수 줄은 19씩 감소 짝수 줄은 20씩 감소
				if ((0 != i % 2)   // 홀수 줄인지 판정
					&& (!m_vecTile[iIndex - (TILEX - 1)]->byOption)) // 홀수 줄의 옵션이 0인지 판정
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - (TILEX - 1)]);
				}
				else if ((0 == i % 2)   // 짝수 줄인지 판정
					&& (!m_vecTile[iIndex - TILEX]->byOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - TILEX]);
				}
			}

			// 왼쪽 하단 대각선
			// 갈 수 있는 옵션(0)인지 조사

			if ((TILEY - 1) != i && (0 != iIndex % (TILEX * 2)))
			{
				//홀수 줄은 20씩 증가 짝수 줄은 19씩 증가
				if ((0 != i % 2)   // 홀수 줄인지 판정
					&& (!m_vecTile[iIndex + TILEX]->byOption)) // 홀수 줄의 옵션이 0인지 판정
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + TILEX]);
				}
				else if ((0 == i % 2)   // 짝수 줄인지 판정
					&& (!m_vecTile[iIndex + (TILEX - 1)]->byOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + (TILEX - 1)]);
				}
			}

			// 오른쪽 하단 대각선
			// 갈 수 있는 옵션(0)인지 조사

			if ((TILEY - 1) != i && ((TILEX * 2 - 1) != iIndex % (TILEX * 2)))
			{
				//홀수 줄은 21씩 증가 짝수 줄은 20씩 증가
				if ((0 != i % 2)   // 홀수 줄인지 판정
					&& (!m_vecTile[iIndex + (TILEX + 1)]->byOption)) // 홀수 줄의 옵션이 0인지 판정
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + (TILEX + 1)]);
				}
				else if ((0 == i % 2)   // 짝수 줄인지 판정
					&& (!m_vecTile[iIndex + TILEX]->byOption))
				{
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + TILEX]);
				}
			}

		}
	}

}
