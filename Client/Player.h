#pragma once
#include "Obj.h"
class CPlayer :
	public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	void		Move_AStar(void);

private:
	wstring			m_wstrStateKey;

};

