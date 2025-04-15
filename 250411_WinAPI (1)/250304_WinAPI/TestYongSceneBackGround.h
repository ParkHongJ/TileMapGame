#pragma once
#include "GameObject.h"

class Image;
class TestYongSceneBackGround : public GameObject
{
public:
	TestYongSceneBackGround();
	virtual ~TestYongSceneBackGround();

	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;

private:
	Image* image;
};

