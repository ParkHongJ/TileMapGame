#pragma once
#include "config.h"
#include "GameObject.h"
#include "CharacterState.h"
#include "IdleState.h"
#include "MoveState.h"
#include "AttackState.h"
#include "InteractionState.h"

#define SUBSTATE 5
#define TOLERANCE 2.0f

class BoxCollider;
class PlayerStatus;
class Item;

enum class SubAnim {
	NONE,
	JUMP_UP,
	JUMP_DOWN,
	HURT_BIRD,
};

struct InputIntent {
	bool moveLeft = false;
	bool moveLeftReleased = false;
	bool moveRight = false;
	bool moveRightReleased = false;
	bool moveDown = false;
	bool moveDownReleased = false;
	bool moveUp = false;
	bool moveUpReleased = false;
	bool jump = false;
	bool attack = false;
	bool interact = false;
	bool shift = false;
	bool bomb = false;

	bool hasMovement() const { return moveLeft || moveRight; }
};

class Character : public GameObject
{
	GENERATE_BODY(Textures/char_yellow.png, 128, 128)
private:
	map<std::pair<unsigned int, unsigned int>, FrameInfo> animationMap;

	Image*				  playerImage;
	Image*          playerFaintEffect; //temp
	CharacterState*				state;


	// Collider
	BoxCollider*		     collider;
	FPOINT			     colliderSize;
	float			  colliderOffsetY;

	// Input 모든 Input 은 여기서 한번에 받아서 사용
	InputIntent				currInput;


	// Item*				 currItem;

	// Stat

	int8_t					   health;

	float						speed;
	float				  attackSpeed;
	float				   attackRate;


	// Render
	bool					   isFlip;
	float					frameTime;
	POINT				 currFrameInd;
	FrameInfo		    currFrameInfo;
	POINT			currFaintFrameInd;
	FrameInfo		currFaintFrameInfo;
	int8_t			  faintBounceTime;

	// Gravity
	float					  gravity;
	float				 maxFallSpeed;
	float				    jumpPower;
	float			    bottomHitDist;


	// State Boolean
	bool			     isMovingAuto;
	bool			      isAttacking;
	bool			      isCrouching;
	bool					isFaint;
	bool			isFallFromHeight;

	// For Camera
	bool			   isLookUpLocked;
	bool			 isLookDownLocked;


	// Time lvalues
	float				 currLockTime;
	float			   lookUpLockTime;
	float			 lookDownLockTime;
	float				currfaintTime;
	float				 maxFaintTime;

	// RayCast
	FPOINT                leftHandPos;
	FPOINT               rightHandPos;

	bool			   isTouchingLeft;
	bool			  isTouchingRight;
	bool				isTouchingTop;
	bool		     isTouchingBottom;

	// AutoHaningMove
	FPOINT            targetHangOnPos;


	// Interaction

	priority_queue<pair<float, GameObject*>>        interActionPQ;
	float								            interactionRadius;
	float							            	interactionOffset;


	// Static State
public:
	static IdleState					idleState;
	static MoveState					moveState;
	static AttackState				  attackState;
	static InteractionState		 interactionState;

private:
	PlayerStatus* playerStatus;


public:
	inline PlayerStatus* GetPlayerStatus() { return playerStatus; };

public:
	// GameObject
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	virtual void Detect(GameObject* obj) override;
	//Add JunYong
	void JunUpdate(float TimeDelta);

private:
	Item* whip;
	Item* holdItem;
	Item* backItem;
	Item* preHoldItem;

	float holdItemHitTime = 0.f;
	float holdItemHitMaxTime = 0.1f;

	// Add Hong
	GameObject* interactionObject = nullptr;
	const float maxPushDelay = 0.55f;
	float pushDelay = maxPushDelay;
public:
	inline Item* GetPreHoldItem() { return preHoldItem; };
//Add JunYong
	// Move

	void Move();
	bool MoveY();
	bool CanGoY(float vy);
	void SetYVelocity(float velocityY) { this->velocity.y = velocityY; }
	void SetXVelocity(float velocityX) { this->velocity.x = velocityX; }
	
	// 
	void Jump();
	void HangOnTile();
	void CheckInterAction();
	bool CheckAlmostFall();
	bool CheckHangOn();
	bool CheckCanPushTile();
	bool CheckCanClimbLadder();
	bool CheckCanClimbRope();
	FPOINT GetHangOnTargetPos();


	// Animation

	void InitAnimationMap();
	void PlayAnimation();
	void HandleAirAnimation();

	void SetAnimationFrameInfo(unsigned int stateClassNum, unsigned int subState);
	void SetFrameTime(float frameTime) { this->frameTime = frameTime; }
	void SetIsLookUpLocked(bool isLookUpLocked) { this->isLookUpLocked = isLookUpLocked; }
	void SetIsLookDownLocked(bool isLookDownLocked) { this->isLookDownLocked = isLookDownLocked; }

	POINT GetCurrFrameInd() const;
	FrameInfo GetCurrFrameInfo() const;

	// HFSM
	void HandleTransitions();

	void HandleIdleLogic();
	void HandleMoveLogic();
	void HandleAttackLogic();
	void HandleInteractionLogic();

	void ChangeState(CharacterState* newState);


	// Gravity
	bool IsAirborne() const;
	void ApplyGravity(float TimeDelta);
	
	// Collision
	void CheckTileCollision();


	// Input
	const InputIntent& GetCurrInputIntent() { return currInput; }
	void HandleInput();
	
	// 
	void OnDamage();




	// Getter & Setter
	void SetJumpPower(float jumpPower) { this->jumpPower = jumpPower; }

	float GetJumpPower() { return this->jumpPower; }
	float GetSpeed() { return this->speed; }

	bool GetIsCrouching() { return isCrouching; }
	bool GetIsLookUpLocked();
	bool GetIsLookDownLocked();
	bool GetCurrAnimEnd();
	
	float GetCurrLockTime() { return this->currLockTime; }
	void  SetCurrLockTime(float lockTime) { this->currLockTime = lockTime; }
	float GetlookUpLockTime() { return this->lookUpLockTime; }
	float GetlookDownLockTime() { return this->lookDownLockTime; }
	bool GetFallFromHeight() { return isFallFromHeight; }
	
	
	float GetVelocitySize();
	float GetYVelocity();

	bool GetIsMovingAuto() const;
	bool GetIsAttacking() { return isAttacking; }

	bool GetIsFaint() { return isFaint; }
	float GetCurrFaintTime() { return currfaintTime; }

	void SetSpeed(float speed) { this->speed = speed; }

	void InitCharacter(string input);


	Character() {};
	virtual ~Character() {};

};