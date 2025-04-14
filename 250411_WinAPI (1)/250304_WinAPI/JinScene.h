#pragma once 
#include "Scene.h"
class JinScene : public Scene
{
public:
	virtual HRESULT Init(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	JinScene() {};
	virtual ~JinScene() {};
};

