#pragma once
#include "config.h"
#include "GameObject.h"
#include "CharacterState.h"
#include "IdleState.h"
#include "MoveState.h"
#include "AttackState.h"
#include "InteractionState.h"

#define ANIMSTATE 5

class BoxCollider;
class PlayerStatus;

enum class SubAnim {
	NONE,
	JUMP_UP,
	JUMP_DOWN,
};

class Character : public GameObject
{
	GENERATE_BODY(Textures/char_yellow.png, 128, 128)
private:
	map<std::pair<unsigned int, unsigned int>, FrameInfo> animationMap;

	BoxCollider* yellowCollider = nullptr;

	CharacterState*				state;

	Image*				  playerImage;

	FPOINT						  dir;
	FPOINT				     velocity;
	RECT			     colliderRect;

	float					frameTime;
	POINT				 currFrameInd;

	FrameInfo		    currFrameInfo;
	FrameInfo           jumpFrameInfo;
	FrameInfo		  attackFrameInfo;
	FrameInfo			ropeFrameInfo;

	float						speed;
	float				  attackSpeed;
	float				   attackRate;

	// Gravity
	float gravity = 1000.f; // 중력 가속도 (픽셀/sec^2)
	float maxFallSpeed = 800.f; // 최대 낙하 속도
	float jumpPower = 500.0f;
	bool                      isInAir;
	
	// 예시: 상승 프레임
	const int JUMP_UP_START = 0;
	const int JUMP_UP_END = 3;

	// 예시: 하강 프레임
	const int JUMP_DOWN_START = 4;
	const int JUMP_DOWN_END = 7;

	// 최대 속도 (예상치 사용)
	const float MAX_JUMP_VEL = 500.f;




	bool					   isFlip;
	bool                  isAttacking;
	bool					  isOnPet;
	
	bool                  jumpPressed;
	bool                attackPressed;

	bool			   isLookUpLocked;
	bool			 isLookDownLocked;


	bool isTouchingLeft = false;
	bool isTouchingRight = false;
	bool isTouchingTop = false;
	bool isTouchingBottom = false;


public:
	static IdleState					idleState;
	static MoveState					moveState;
	static AttackState				  attackState;
	static InteractionState		 interactionState;

/// <summary>
/// Add JunYong
/// </summary>
private:
	PlayerStatus* playerStatus;
public:


public:

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;


	// Move


	void SetDir(FPOINT dir) { this->dir = dir; }
	void Move();
	void SetYVelocity(float velocityY) { this->velocity.y = velocityY; }
	void SetXVelocity(float velocityX) { this->velocity.x = velocityX; }
	
	void SetIsInAir(bool isInAir) { this->isInAir = isInAir; }
	bool GetIsinAir() { return this->isInAir; }
	void SetJumpPower(float jumpPower) { this->jumpPower = jumpPower; }

	float GetJumpPower() { return this->jumpPower; }
	float GetSpeed() { return this->speed; }

	// Animation

	void InitAnimationMap();

	void PlayAnimation();

	void SetAnimationFrameInfo(unsigned int stateClassNum, unsigned int subState);
	void SetFrameTime(float frameTime) { this->frameTime = frameTime; }
	void SetIsLookUpLocked(bool isLookUpLocked) { this->isLookUpLocked = isLookUpLocked; }
	void SetIsLookDownLocked(bool isLookDownLocked) { this->isLookDownLocked = isLookDownLocked; }

	POINT GetCurrFrameInd() const;
	FrameInfo GetCurrFrameInfo() const;
	bool GetIsLookUpLocked();
	bool GetIsLookDownLocked();
	bool GetCurrAnimEnd();

	// Gravity
	
	void ApplyGravity(float TimeDelta);
	
	void CheckCollision();

	
	float GetVelocitySize();
	float GetYVelocity();


	void SetSpeed(float speed) { this->speed = speed; }
	
	void ChangeState(CharacterState* newState);

	bool PressAnyKey();

	RECT GetColliderSize() { return { colliderRect.left,colliderRect.top,colliderRect.right,colliderRect.bottom }; }
	

	Character() {};
	virtual ~Character() {};

};