#pragma once
#include "GameObject.h"

enum class MonsterState
{
	IDLE,
	MOVE,
	ATTACKMOVE,
	ATTACK
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
	virtual void CheckCollision();
	virtual void Move();
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);

protected:

	MonsterState monsterState;
	
	FPOINT dir;
	POINT currFrame;
	FrameInfo moveFrameInfo;
	FrameInfo attackFrameInfo;
	FrameInfo attackMoveInfo;
	FrameInfo currFrameInfo;

	int damage;
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

	bool isTouchingLeft;
	bool isTouchingRight;
	bool isTouchingTop;
	bool isTouchingBottom;

};
