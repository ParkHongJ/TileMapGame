#pragma once
#include "Scene.h"
class TestAnimationObject;

class YongScene : public Scene
{
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(HDC hdc) override;

	YongScene() {};
	virtual ~YongScene() {};

private:
	TestAnimationObject* Test;
};

