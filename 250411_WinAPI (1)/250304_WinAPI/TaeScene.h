#pragma once
#include "Scene.h"
#include "GameObject.h"

class Image;
class Character; 

class TaeScene : public Scene
{
private:

	Image* backGround;

	Character* yellow;


public:
	virtual HRESULT Init(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	TaeScene() {};
	virtual ~TaeScene() {};

};

