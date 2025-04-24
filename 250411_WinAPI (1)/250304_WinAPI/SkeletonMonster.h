#pragma once
#include "Monster.h"

class BoxCollider;
class Image;
class SkeletonMonster : public Monster
{
	GENERATE_BODY(Textures/Monster/monsters.png, 128, 128)

public:
	SkeletonMonster();
	~SkeletonMonster();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void FrameUpdate(float TimeDelta) override;
	virtual void CheckTileCollision() override;
	virtual void CheckPlayerCollision() override;
	virtual void CheckItemCollision() override;
	virtual void Move() override;
	virtual void ApplyGravity(float TimeDelta) override;
	virtual void ReverseMove();
	virtual void Detect(GameObject* obj) override;
	virtual void DeadEvent(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

private:
	Image* skeletonImage;
	BoxCollider* skeletonCollider = nullptr;
	FPOINT colliderSize;
	float colliderOffsetY;
};
