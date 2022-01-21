#pragma once

#include "SingleTexture.h"
#include "MultiTexture.h"
#include "Include.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEXINFO*		Get_Texture(const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0);
	const wstring&		Get_String() { return m_wstFullPath; }
	void				Set_String(wstring wstrFullPath) { m_wstFullPath = wstrFullPath; }

public:
	HRESULT				ReadImgPath(const wstring& wstrPath);
	HRESULT				InsertTexture(TEXTYPE eType, const TCHAR* pFilePath, const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0); 
	void				Release(void);

private:
	map<wstring, CTexture*>			m_mapTexture;

private:
	wstring					m_wstFullPath;

};

