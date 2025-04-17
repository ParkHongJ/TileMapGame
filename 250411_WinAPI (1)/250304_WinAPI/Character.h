#pragma once
#include "config.h"
#include "GameObject.h"
#include "CharacterState.h"
#include "IdleState.h"
#include "MoveState.h"
#include "AttackState.h"
#include "InteractionState.h"

class Character : public GameObject
{

private:
	map<std::pair<unsigned int, unsigned int>, FrameInfo> animationMap;

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
	float maxFallSpeed = 2000.f; // 최대 낙하 속도
	bool                      isInAir;



	bool					   isFlip;
	bool                  isAttacking;
	bool					  isOnPet;
	
	bool                  jumpPressed;
	bool                attackPressed;

	bool			   isLookUpLocked;
	bool			 isLookDownLocked;


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

	void Move(int dirX);


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
	
	bool CheckGround();

	
	float GetVelocitySize();
	float GetYVelocity();


	void SetSpeed(float speed) { this->speed = speed; }
	
	void ChangeState(CharacterState* newState);

	bool PressAnyKey();

	RECT GetColliderSize() { return { colliderRect.left,colliderRect.top,colliderRect.right,colliderRect.bottom }; }
	

	Character() {};
	virtual ~Character() {};

};