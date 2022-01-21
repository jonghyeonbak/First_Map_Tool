#include "stdafx.h"
#include "SceneMgr.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	:m_pScene(nullptr)
	, m_eCurScene(END)
	, m_eNextScene(END)
{
}


CSceneMgr::~CSceneMgr()
{
	Release_SceneMgr();
}

HRESULT CSceneMgr::Change_SceneMgr(ID eID)
{
	m_eNextScene = eID; 
	if (m_eCurScene != m_eNextScene)
	{
		Safe_Delete(m_pScene); 

		switch (m_eNextScene)
		{
		case CSceneMgr::LOADING:
			m_pScene = CLogo::Create();
			break;
		case CSceneMgr::STAGE:
			m_pScene = CStage::Create(); 
			break;
		case CSceneMgr::BOSS:
			break;
		default:
			break;
		}
		if (nullptr == m_pScene)
		{
			MSG_BOX(L"SceneChange Failed"); 
			return E_FAIL; 
		}
		m_eCurScene = m_eNextScene; 
	}
	return S_OK;
}

void CSceneMgr::Update_SceneMgr()
{
	m_pScene->Update_Scene(); 
}

void CSceneMgr::Late_Update_SceneMgr()
{
	m_pScene->Late_Update_Scene(); 
}

void CSceneMgr::Render_SceneMgr()
{
	m_pScene->Render_Scene(); 
}

void CSceneMgr::Release_SceneMgr()
{
	Safe_Delete(m_pScene); 
}
