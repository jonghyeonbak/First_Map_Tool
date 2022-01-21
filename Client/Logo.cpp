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

	// �̹����� �߽� ��ǥ
	float	fCenterX = pTextureInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTextureInfo->tImgInfo.Height / 2.f;


	D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 2.f, 0.f);


	CDevice::GetInstance()->Get_Sprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->Get_Sprite()->Draw(pTextureInfo->pTexture,
		nullptr,	// ����� �̹��� ������ ���� ��Ʈ ����ü �ּҰ�, null�� ��� �̹����� 0,0�� �������� ����ϰԵ�
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),	// ����� �̹����� �߽� �� ��ǥ�� vec3 Ÿ���� �ּҰ�, null�� ��� 0,0�� �߽� ��ǥ�� ��
		nullptr,	// ��ġ ��ǥ�� ���� vec3Ÿ���� �ּҰ�, null�� ��� ��ũ�� ���� 0, 0 ��ǥ ���
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

	CTextureMgr::GetInstance()->Set_String(L"�ε� �Ϸ�");

	LeaveCriticalSection(&pLogo->Get_Crt());

	// _endthreadex(0); return 0�� �� ��� �ڵ����� _endthreadex�Լ��� ȣ��

	return 0;
}

CLogo* CLogo::Create(void)
{
	CLogo*		pInstance = new CLogo;

	if (FAILED(pInstance->Ready_Scene()))
		return nullptr;

	return pInstance;
}
