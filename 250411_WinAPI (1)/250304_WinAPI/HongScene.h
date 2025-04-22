#pragma once
#include "Scene.h"
class DummyHongPlayer;
class HongScene : public Scene
{
private:
	float FrameTime = 0.f;
	int currentFrame = 0;

	
	class ArrowTrap* arrowTrap;

	class Image* background;
public:
	virtual HRESULT Init(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void LateUpdate(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

public:
	HongScene() {};
	virtual ~HongScene() {};
};
