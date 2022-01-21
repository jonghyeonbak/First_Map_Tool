#pragma once

#include "Include.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual HRESULT		Initialize(void)PURE;
	virtual int			Update(void)PURE;
	virtual void		Late_Update(void)PURE;
	virtual void		Render(void)PURE;
	virtual void		Release(void)PURE;

protected:
	void				Move_Frame(void);

protected:
	static D3DXVECTOR3			m_vScroll;
	INFO						m_tInfo;
	FRAME						m_tFrame;

	bool						m_bInit;
	wstring						m_wstrObjKey;

};

