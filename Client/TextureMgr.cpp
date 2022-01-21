#include "stdafx.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO * CTextureMgr::Get_Texture(const TCHAR* pObjKey, const TCHAR * pStateKey, const int & iCnt)
{
	auto	iter = find_if(m_mapTexture.begin(),
		m_mapTexture.end(),
		[&](auto& MyPair)->bool
	{
		if (pObjKey == MyPair.first)
			return true;

		return false;
	});

	if (iter == m_mapTexture.end())
		return nullptr;


	return iter->second->Get_Texture(pStateKey, iCnt);
}

HRESULT CTextureMgr::ReadImgPath(const wstring& wstrPath)
{
	wifstream		fin;
	fin.open(wstrPath);

	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR	szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MAX_STR] = L"";
		TCHAR	szPath[MAX_PATH] = L"";

		while (true)
		{
			// '|' ������ ��� ���ڿ� �Է� ó��

			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			// ���ڿ��� ������ ��ȯ �Լ�
			int iCount = _ttoi(szCount);

			if (FAILED(InsertTexture(TEX_MULTI,  szPath, szObjKey, szStateKey, iCount)))
			{
				MSG_BOX(L"Client TextureMgr Texture Load Failed");
				return E_FAIL;
			}

		}

		fin.close();		// close �Լ��� ���� ����(��ü Ÿ���̾ �Ҹ� ������ �˾Ƽ� ������ ���� ���� �Ҹ� ����)
	}


	return S_OK;
}

HRESULT CTextureMgr::InsertTexture(TEXTYPE eType, const TCHAR * pFilePath, const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCnt)
{
	m_wstFullPath = pFilePath;

	auto	iter = find_if(m_mapTexture.begin(),
						   m_mapTexture.end(),
							[&](auto& MyPair)->bool
						{
							if (pObjKey == MyPair.first)
								return true;

							return false;
						});

	if (iter == m_mapTexture.end())
	{
		CTexture*		pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->InsertTexture(pFilePath, pStateKey, iCnt)))
		{
			MSG_BOX(pFilePath);
			return E_FAIL;
		}

		m_mapTexture.emplace(pObjKey, pTexture);
	}
	else if (TEX_MULTI == eType)
		iter->second->InsertTexture(pFilePath, pStateKey, iCnt);

	return S_OK;
}

void CTextureMgr::Release(void)
{
	for_each(m_mapTexture.begin(), m_mapTexture.end(), 
		[](auto& MyPair)
	{
		Safe_Delete(MyPair.second);
	});

	m_mapTexture.clear();

}
