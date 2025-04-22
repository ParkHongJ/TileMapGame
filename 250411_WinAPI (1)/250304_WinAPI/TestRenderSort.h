#pragma once
#include "GameObject.h"
class Image;

class TestRenderSort : public GameObject
{
public:
	TestRenderSort() {};
	virtual ~TestRenderSort() {};

	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;

private:
	Image* image;

};

