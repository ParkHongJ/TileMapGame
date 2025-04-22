#pragma once
#include "Scene.h"
class TestAnimationObject;
class YongScene : public Scene
{
public:
	virtual HRESULT Init(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	YongScene() {};
	virtual ~YongScene() {};
	
private:
	TestAnimationObject* Test;
};

