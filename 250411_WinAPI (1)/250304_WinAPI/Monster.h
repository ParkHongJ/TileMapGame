#pragma once
#include "GameObject.h"

enum class MonsterState
{
	IDLE,		
	MOVE,
	ATTACKMOVE,
	ATTACK,
	WAITATTACK,
	DEAD
};
class Character;
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
	virtual void MeetPlayer();
	virtual void Move();
	virtual void ApplyGravity(float TimeDelta);
	virtual void Detect(GameObject* obj);
	virtual void DeadEvent(float TimeDelta);
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	virtual int GetDamage() { return damage; }
	virtual void SetDamage(int damage) { this->damage = damage; }

public:
	float RandomRange(float min, float max)
	{
		float r = (float)rand() / RAND_MAX; // 0.0 ~ 1.0
		return min + (max - min) * r;
	}

	void ClampVector(FPOINT& vec, float maxLength)
	{
		float len = sqrtf(vec.x * vec.x + vec.y * vec.y);
		if (len > maxLength)
		{
			float scale = maxLength / len;
			vec.x *= scale;
			vec.y *= scale;
		}
	}

	FPOINT RotateVector(const FPOINT& vec, float degrees)
	{
		float rad = degrees * 3.1415926f / 180.0f;
		float cosA = cosf(rad);
		float sinA = sinf(rad);

		return {
			vec.x * cosA - vec.y * sinA,
			vec.x * sinA + vec.y * cosA
		};
	}

	void AddForce(const FPOINT& force)
	{
		totalForce.x += force.x;
		totalForce.y += force.y;
	}

	float Dot(const FPOINT& a, const FPOINT& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	FPOINT Reflect(const FPOINT& velocity, const FPOINT& normal)
	{
		float dot = Dot(velocity, normal);
		return {
			velocity.x - 2.0f * normal.x * dot,
			velocity.y - 2.0f * normal.y * dot
		};
	}

	void MoveJumpStart(float speed, float angle, float mass = 1.0f, FPOINT gravity = { 0, 98.0f });
	void MoveJump(float TimeDelta);

protected:

	MonsterState monsterState;
	Character* player;
	
	FPOINT dir;
	POINT currFrame;
	FrameInfo IdleFrameInfo;
	FrameInfo moveFrameInfo;
	FrameInfo attackFrameInfo;
	FrameInfo attackMoveInfo;
	FrameInfo attackMoveStartInfo;
	FrameInfo deadFrameInfo;
	FrameInfo currFrameInfo;

	int damage;
	int monsterHP;
	float moveSpeed;
	// float gravity = 1000.f; // 중력 가속도 (픽셀/sec^2)
	float maxFallSpeed = 800.f; // 최대 낙하 속도
	float deadElipsedTime = 0.0f;
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

	bool isFight = false;
	protected:

	FPOINT velocity;
	FPOINT acceleration = { 0, 0 };  // 가속도

	float mass = 1.0f;
	FPOINT gravity = { 0, 98.0f };  // 중력
	bool useGravity = false;
	bool bPhysics = false;
	FPOINT totalForce = { 0, 0 };
	float bounciness = 0.4f;
	bool isInAir = bPhysics || velocity.y != 0.f;

	float attackCool = 0.0f;
	float attackDuration = 0.f;	
	float attackCoolTime = 1.5f;	// 총 쿨 타임
};
