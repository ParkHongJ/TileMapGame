#pragma once
#include "Scene.h"
#include "GameObject.h"

class Image;
class Character; 
class BoxCollider;

class TaeScene : public Scene
{
	GENERATE_BODY(Textures/char_yellow.png, 128, 128)

private:

	Character* yellow;
	BoxCollider* yellowCollider = nullptr;


public:
	virtual HRESULT Init(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	TaeScene() {};
	virtual ~TaeScene() {};

};

