#pragma once

#include "Include.h"

#include "Logo.h"
#include "Stage.h"

class CScene; 
class CSceneMgr final
{
	DECLARE_SINGLETON(CSceneMgr)

public:
	enum ID {LOADING, STAGE, BOSS, END};
private:
	CSceneMgr();
	~CSceneMgr();
public:
	HRESULT Change_SceneMgr(ID eID);
public:
	void Update_SceneMgr();
	void Late_Update_SceneMgr();
	void Render_SceneMgr();
	void Release_SceneMgr();

private:
	CScene* m_pScene; 
	ID m_eCurScene; 
	ID m_eNextScene; 
};

