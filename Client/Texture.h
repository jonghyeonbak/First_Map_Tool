#pragma once

#include "Include.h"

class CTexture
{
public:
	CTexture();
	virtual ~CTexture();

public:
	virtual const TEXINFO*		Get_Texture(const TCHAR* pStateKey = L"", const int& iCnt = 0)PURE;
public:
	virtual HRESULT		InsertTexture(const TCHAR* pFilePath,  // 파일 경로
									  const TCHAR* pStateKey = L"", // 멀티 텍스처일 경우 사용, 스프라이트 상태 키값
									  const int& iCnt = 0)PURE;     // 멀티 텍스처일 경우 사용, 몇장있는가

	virtual void		Release(void)PURE;
};

