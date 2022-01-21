#pragma once

#include "MyTerrain.h"
#include "Scene.h"

class CStage : public CScene
{
private:
	CStage();
	~CStage();

public:
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	static CStage*		Create(void);

};

