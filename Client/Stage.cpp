#include "stdafx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "Player.h"


CStage::CStage()
{
}


CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{
	/*if (FAILED(CTextureMgr::GetInstance()->InsertTexture(TEX_MULTI, L"../Texture/Stage/Terrain/Tile/Tile%d.png",
		L"Terrain",
		L"Tile",
		36)))
	{
		MSG_BOX(L"Tile Texture Insert Failed");
		return E_FAIL;
	}*/

	


	CObj*		pObj = new CMyTerrain;

	if(nullptr != pObj)
		pObj->Initialize();

	CObjMgr::GetInstance()->Add_Object(CObjMgr::TERRAIN, pObj);
	
	pObj = new CPlayer;
	pObj->Initialize();
	CObjMgr::GetInstance()->Add_Object(CObjMgr::PLAYER, pObj);

	return S_OK;
}

void CStage::Update_Scene()
{
	CObjMgr::GetInstance()->Update();
}

void CStage::Late_Update_Scene()
{
	CObjMgr::GetInstance()->Late_Update();
}

void CStage::Render_Scene()
{
	CObjMgr::GetInstance()->Render();
}

void CStage::Release_Scene()
{
	
}

CStage* CStage::Create(void)
{
	CStage*		pInstance = new CStage;

	if (FAILED(pInstance->Ready_Scene()))
		return nullptr;

	return pInstance;
}
