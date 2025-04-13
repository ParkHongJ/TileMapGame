#pragma once
#include "Scene.h"
class TaeScene : public Scene
{
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(HDC hdc) override;

	TaeScene() {};
	virtual ~TaeScene() {};

};

