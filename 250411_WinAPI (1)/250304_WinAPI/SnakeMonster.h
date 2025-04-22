#pragma once
#include "Monster.h"

class BoxCollider;
class Image;
class SnakeMonster : public Monster
{
	GENERATE_BODY(Textures/Monster/SnakeMonster.png, 128, 128)
		
public : 
	SnakeMonster();
	~SnakeMonster();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void FrameUpdate(float TimeDelta) override;
	virtual void CheckTileCollision() override; 
	virtual void CheckPlayerCollision() override;
	virtual void CheckItemCollision() override;
	virtual void Move() override;
	virtual void ReverseMove();
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

private: 
	Image* snakeImage;
	BoxCollider* snakeCollider = nullptr;
	FPOINT colliderSize;
	float colliderOffsetY;
};

