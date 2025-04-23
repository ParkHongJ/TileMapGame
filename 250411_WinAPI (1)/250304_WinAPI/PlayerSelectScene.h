#pragma once
#include "Scene.h"
class PlayerSelectScene : public Scene
{
private:

public:
	virtual HRESULT Init(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void LateUpdate(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	PlayerSelectScene() {};
	virtual ~PlayerSelectScene() {};

	
};

