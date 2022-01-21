#include "stdafx.h"
#include "Logo.h"
#include "SceneMgr.h"
#include "TextureMgr.h"
#include "Device.h"


CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release_Scene();
}

HRESULT CLogo::Ready_Scene()
{
	InitializeCriticalSection(&m_Crt);

	int	a = 10;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ImgLoadThreadFunc, this, 0, nullptr);



	return S_OK;
}

void CLogo::Update_Scene()
{
	if (GetAsyncKeyState(VK_RETURN))
	{
		CSceneMgr::GetInstance()->Change_SceneMgr(CSceneMgr::STAGE);
		return;
	}

}

void CLogo::Late_Update_Scene()
{

}

void CLogo::Render_Scene()
{
	const	TEXINFO*		pTextureInfo = CTextureMgr::GetInstance()->Get_Texture(L"Logo");
	if (nullptr == pTextureInfo)
		return;

	D3DXMATRIX	matTrans;

	// 이미지의 중심 좌표
	float	fCenterX = pTextureInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTextureInfo->tImgInfo.Height / 2.f;


	D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 2.f, 0.f);


	CDevice::GetInstance()->Get_Sprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->Get_Sprite()->Draw(pTextureInfo->pTexture,
		nullptr,	// 출력할 이미지 영역에 대한 렉트 구조체 주소값, null인 경우 이미지의 0,0을 기준으로 출력하게됨
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),	// 출력할 이미지의 중심 축 좌표를 vec3 타입의 주소값, null인 경우 0,0이 중심 좌표가 됨
		nullptr,	// 위치 좌표에 대한 vec3타입의 주소값, null인 경우 스크린 상의 0, 0 좌표 출력
		D3DCOLOR_ARGB(255, 255, 255, 255));


	wstring		wstrFullPath = CTextureMgr::GetInstance()->Get_String();
	D3DXMatrixTranslation(&matTrans, 50.f, 500.f, 0.f);
	CDevice::GetInstance()->Get_Sprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->Get_Font()->DrawTextW(CDevice::GetInstance()->Get_Sprite(),
		wstrFullPath.c_str(),
		wstrFullPath.length(),
		nullptr,
		0,
		D3DCOLOR_ARGB(255, 255, 0, 0));


}

void CLogo::Release_Scene()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);
}

size_t	CALLBACK CLogo::ImgLoadThreadFunc(void* pArg)
{
	CLogo*		pLogo = reinterpret_cast<CLogo*>(pArg);

	EnterCriticalSection(&pLogo->Get_Crt());

	/*switch (eID)
	{
	case STAGE1:
		break;

	case STAGE2:
		break;

	case STAGE3:
		break;
	}*/

	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(TEX_SINGLE, L"../Texture/JusinLogo.png",
		L"Logo")))
	{
		MSG_BOX(L"Logo Texture Insert Failed");
		return E_FAIL;
	}

	if (FAILED(CTextureMgr::GetInstance()->ReadImgPath(L"../Data/ImgPath.txt")))
	{
		MSG_BOX(L"Texture Insert Failed");
		return E_FAIL;
	}

	CTextureMgr::GetInstance()->Set_String(L"로딩 완료");

	LeaveCriticalSection(&pLogo->Get_Crt());

	// _endthreadex(0); return 0을 할 경우 자동으로 _endthreadex함수를 호출

	return 0;
}

CLogo* CLogo::Create(void)
{
	CLogo*		pInstance = new CLogo;

	if (FAILED(pInstance->Ready_Scene()))
		return nullptr;

	return pInstance;
}
