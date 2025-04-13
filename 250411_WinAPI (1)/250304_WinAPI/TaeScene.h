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
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(HDC hdc) override;

	TaeScene() {};
	virtual ~TaeScene() {};

};

