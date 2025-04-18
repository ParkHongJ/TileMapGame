#pragma once
#include "Monster.h"

class BoxCollider;
class Image;
class SnakeMonster : public Monster
{
public : 
	SnakeMonster();
	~SnakeMonster();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void FrameUpdate(float TimeDelta) override;
	virtual void Move() override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

private: 
	Image* snakeImage;
	BoxCollider* snakeCollider = nullptr;


};

