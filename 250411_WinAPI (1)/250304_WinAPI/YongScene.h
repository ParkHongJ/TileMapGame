#pragma once
#include "Scene.h"
class TestAnimationObject;
class Image;

class YongScene : public Scene
{
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	YongScene() {};
	virtual ~YongScene() {};

	

private:
	TestAnimationObject* Test;

	Image* backGround;
};

