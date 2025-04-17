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

	bool					   isFlip;
	bool                      isInAir;
	bool                  isAttacking;
	bool					  isOnPet;


	
	bool                  jumpPressed;
	bool                attackPressed;

	bool			   isLookUpPaused;
	bool			 isLookDownPaused;




public:
	static IdleState idleState;
	static MoveState moveState;
	static AttackState attackState;
	static InteractionState interactionState;

	

public:

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

		
	void InitAnimationMap();


	void SetAnimationFrameInfo(unsigned int stateClassNum, unsigned int subState);
	void SetFrameTime(float frameTime) { this->frameTime = frameTime; }
	void SetIsLookUpPaused(bool isLookUpPaused) { this->isLookUpPaused = isLookUpPaused; }
	void SetIsLookDownPaused(bool isLookDownPaused) { this->isLookDownPaused = isLookDownPaused; }
	void SetSpeed(float speed) { this->speed = speed; }
	
	void PlayAnimation();
	void ChangeState(CharacterState* newState);

	bool PressAnyKey();

	float GetVelocitySize();
	float GetYVelocity();
	bool GetIsLookUpPaused();
	bool GetIsLookDownPaused();
	POINT GetCurrFrameInd() const;
	FrameInfo GetCurrFrameInfo() const;

	void Move(int dirX);
	void LookUp();
	void LookDown();
	
	Character() {};
	virtual ~Character() {};

};