#pragma once
#include "GameObject.h"

enum class MonsterState
{
	IDLE,
	MOVE,
	ATTACKMOVE,
	ATTACK,
	DEAD
};
class Monster : public GameObject
{
public:
	Monster();
	virtual ~Monster();

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update(float TimeDelta);
	virtual void FrameUpdate(float TimeDelta);
	virtual void CheckTileCollision();
	virtual void CheckPlayerCollision();
	virtual void CheckItemCollision();
	virtual void Move();
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);
	virtual void Detect(GameObject* obj) override;

	virtual int GetDamage() { return damage; }
	virtual void SetDamage(int damage) { this->damage = damage; }

protected:

	MonsterState monsterState;
	
	FPOINT dir;
	POINT currFrame;
	FrameInfo IdleFrameInfo;
	FrameInfo moveFrameInfo;
	FrameInfo attackFrameInfo;
	FrameInfo attackMoveInfo;
	FrameInfo currFrameInfo;

	int damage;
	int monsterHP;
	float moveSpeed;
	float gravity = 1000.f; // 중력 가속도 (픽셀/sec^2)
	float maxFallSpeed = 800.f; // 최대 낙하 속도
	//float jumpPower = 500.0f;
	//bool isInAir;

	int startFrameIndexX;
	int startFrameIndexY;

	int endFrameIndexX;
	int endFrameIndexY;

	int curFrameIndexX;
	int curFrameIndexY;

	float elipsedTime;
	float frameSpeed;

	bool meetPlayer;
	bool isAttack;
	bool isDamaged;
	bool isFlip;
	bool meetWall = false;
	bool meetPlayerLeft;
	bool meetPlayerRight;
	bool hasBottomTile;

	bool isTouchingLeft;
	bool isTouchingRight;
	bool isTouchingTop;
	bool isTouchingBottom;

	bool isTileTouchingLeft;
	bool isTileTouchingRight;
	bool isTileTouchingTop;
	bool isTileTouchingLeftBottom;
	bool isTileTouchingRightBottom;

	bool isPlayerTouchingLeft;
	bool isPlayerTouchingRight;
	bool isPlayerTouchingTop;
	bool isPlayerTouchingBottom;
	bool isPlayerTouchingCenterTop;

	bool isItemTouchingLeft;
	bool isItemTouchingRight;
	bool isItemTouchingTop;
	bool isItemTouchingBottom;
};
