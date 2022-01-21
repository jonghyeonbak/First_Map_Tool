#include "stdafx.h"
#include "MainGame.h"
#include "Device.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "TimeMgr.h"
#include "AStarMgr.h"

CMainGame::CMainGame()
	: m_pGraphicDev(CDevice::GetInstance())
{
}


CMainGame::~CMainGame()
{
	Release();
}


HRESULT CMainGame::Initialize(void)
{
	if (FAILED(m_pGraphicDev->InitDevice()))
	{
		MSG_BOX(L"Client Device Create Failed");
		return E_FAIL;
	}

	CSceneMgr::GetInstance()->Change_SceneMgr(CSceneMgr::LOADING);
	CTimeMgr::GetInstance()->Initialize();

	return S_OK;
}

void CMainGame::Update(void)
{
	CTimeMgr::GetInstance()->Update();
	CSceneMgr::GetInstance()->Update_SceneMgr();
}

void CMainGame::Late_Update(void)
{
	CSceneMgr::GetInstance()->Late_Update_SceneMgr();
}

void CMainGame::Render(void)
{
	m_pGraphicDev->Render_Begin();

	CSceneMgr::GetInstance()->Render_SceneMgr();

	m_pGraphicDev->Render_End();
}

void CMainGame::Release(void)
{
	CAStarMgr::GetInstance()->DestroyInstance();
	CTimeMgr::GetInstance()->DestroyInstance();
	CObjMgr::GetInstance()->DestroyInstance();
	CTextureMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	m_pGraphicDev->DestroyInstance();
}

