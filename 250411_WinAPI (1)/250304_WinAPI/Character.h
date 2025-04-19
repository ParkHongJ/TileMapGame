#pragma once
#include "config.h"
#include "GameObject.h"
#include "CharacterState.h"
#include "IdleState.h"
#include "MoveState.h"
#include "AttackState.h"
#include "InteractionState.h"

#define ANIMSTATE 5
#define TOLERANCE 2.0f

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

	Image*				  playerImage;
	CharacterState*				state;

	BoxCollider*		     collider;

	//Item*				     currItem;

	FPOINT				     velocity;


	bool					   isFlip;
	float					frameTime;
	POINT				 currFrameInd;
	FrameInfo		    currFrameInfo;

	float						speed;
	float				  attackSpeed;
	float				   attackRate;

	// Gravity
	float					  gravity;
	float				 maxFallSpeed;
	float				    jumpPower;
	bool                      isInAir;


	bool				isHangOn;
	bool                  isAttacking;
	bool					  isOnVehicle;
	
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

	bool				isOnLadder;
	bool				isOnRope;


	float	bottomHitDist = 10000.0f; 


	FPOINT leftHandPos;
	FPOINT rightHandPos;
	


	bool isMovingAuto = false;
	FPOINT targetHangOnPos = { 0.f, 0.f };

	FPOINT colliderSize;
	float colliderOffsetY;

	priority_queue<pair<float, GameObject*>> interActionPQ;
	float								interactionRadius;
	float								interactionOffset;



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
	// Header (Character.h)에 선언
	bool GetIsHangOn() const;
	void SetIsHangOn(bool value);

		bool GetIsMovingAuto() const;
		void SetIsMovingAuto(bool value);

	void Move();
	bool MoveY();
	bool CanGoY(float vy);
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
	

	// 사다리 상태
	void SetIsOnLadder(bool value) { isOnLadder = value; }
	bool GetIsOnLadder() const { return isOnLadder; }

	// 밧줄 상태
	void SetIsOnRope(bool value) { isOnRope = value; }
	bool GetIsOnRope() const { return isOnRope; }


	bool CheckAlmostFall();
	bool CheckHangOn();

	void CheckTileCollision();

	// HFSM
	void HandleIdleLogic();
	void HandleMoveLogic();
	void HandleAttackLogic();
	void HandleInteractionLogic();


	FPOINT GetHangOnTargetPos();



	// Gravity
	
	void ApplyGravity(float TimeDelta);
	

	
	float GetVelocitySize();
	float GetYVelocity();


	

	void SetSpeed(float speed) { this->speed = speed; }
	
	void ChangeState(CharacterState* newState);

	bool PressAnyKey();

	Character() {};
	virtual ~Character() {};

};