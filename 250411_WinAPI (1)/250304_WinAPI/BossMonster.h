#pragma once
#include "Monster.h"

class Tile;
class BoxCollider;
class Image;
class BossMonster : public Monster
{
	GENERATE_BODY(Textures/Monster/boss.png, 128, 128)

public:
	BossMonster();
	~BossMonster();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void FrameUpdate(float TimeDelta) override;
	virtual void CheckTileCollision() override;
	virtual void CheckPlayerCollision() override;
	virtual void CheckItemCollision() override;
	virtual void MeetPlayer() override;
	virtual void Move() override;
	virtual void ApplyGravity(float TimeDelta) override;
	virtual void ReverseMove();
	virtual void Detect(GameObject* obj) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

private:
	Image* bossImage;
	BoxCollider* bossCollider = nullptr;
	Tile* tile;
	FPOINT colliderSize;
	float colliderOffsetY;


	FPOINT playerPos;
	FPOINT playerHeight;
	FPOINT playerPosLeftBottom;
	FPOINT playerPosRightBottom;
	float playerPosBottom;

	FPOINT monsterPosLeftTop;
	FPOINT monsterPosRightTop;
	float monsterPosTop;

	float attackDuration = 0.f;
	float attackTime = 0.5f;
};

