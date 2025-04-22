#pragma once
#include "Scene.h"
#include "GameObject.h"

class Image;
class Character; 
class BoxCollider;

class TaeScene : public Scene
{
private:

	Character* yellow;
	BoxCollider* yellowCollider = nullptr;


public:
	virtual HRESULT Init(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	TaeScene() {};
	virtual ~TaeScene() {};

};

