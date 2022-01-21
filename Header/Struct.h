#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3	vPos;
	D3DXVECTOR3	vDir;
	D3DXVECTOR3 vLook;
	D3DXMATRIX	matWorld;

}INFO;

typedef struct tagTexture
{
	// �̹��� �� ���� �����ϴ� com��ü
	LPDIRECT3DTEXTURE9		pTexture;

	// �̹��� ������ �����ϱ� ���� ����ü
	D3DXIMAGE_INFO			tImgInfo;

}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3	vPos;
	D3DXVECTOR3 vSize;

	BYTE		byOption;
	BYTE		byTileIdx;

	// ���� Ÿ���� �ε���
	int			iIndex;
	// �θ� Ÿ���� �ε���
	int			iParentIndex;

}TILE;

typedef struct tagUnitData
{
#ifndef _AFX
	wstring			strName;
#else
	CString		strName;
#endif

	int			iAttack;
	int			iDef;
	BYTE		byJobIndex;
	BYTE		byItem;

}UNITDATA;


typedef struct tagTexturePath
{
	wstring			wstrObjKey = L"";
	wstring			wstrStateKey = L"";
	wstring			wstrPath = L"";
	int				iCount = 0;

}IMGPATH;

typedef struct tagFrame
{
	tagFrame(){}
	tagFrame(float _fFrame, float _fMax)
		: fFrame(_fFrame), fMax(_fMax)
	{}

	float   fFrame;	// ���� ��µǴ� �̹��� �ε���
	float	fMax;	// �ִ� �̹��� ���
}FRAME;