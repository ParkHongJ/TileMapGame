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

	CharacterState*				state;

	BoxCollider* yellowCollider;

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
	float					  gravity;
	float				 maxFallSpeed;
	float				    jumpPower;
	bool                      isInAir;


	bool					   isFlip;
	bool                  isAttacking;
	bool					  isOnPet;
	
	bool                  jumpPressed;
	bool                attackPressed;

	bool			   isLookUpLocked;
	bool			 isLookDownLocked;

	float				 currLockTime;
	float			   lookUpLockTime;
	float			 lookDownLockTime;



	bool			   isTouchingLeft;
	bool			  isTouchingRight;
	bool				isTouchingTop;
	bool		     isTouchingBottom;

	float bottomHitDist = 10000.0f; // 캐릭터에서 땅까지의 거리 (기본은 아주 큰 값)

	float colliderSize;
	float colliderOffset;

public:
	static IdleState					idleState;
	static MoveState					moveState;
	static AttackState				  attackState;
	static InteractionState		 interactionState;

	

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
	bool GetIsInAir() { return this->isInAir; }
	void SetJumpPower(float jumpPower) { this->jumpPower = jumpPower; }

	float GetJumpPower() { return this->jumpPower; }
	float GetSpeed() { return this->speed; }


	void HandleTransitions();
	void HandleAirAnimation();
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
	
	float GetCurrLockTime() { return this->currLockTime; }
	void  SetCurrLockTime(float lockTime) { this->currLockTime = lockTime; }
	float GetlookUpLockTime() { return this->lookUpLockTime; }
	float GetlookDownLockTime() { return this->lookDownLockTime; }

	void SetIsAttacking(bool input) { this->isAttacking = input; }
	


	// HFSM
	void HandleIdleLogic();
	void HandleMoveLogic();
	void HandleAttackLogic();



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