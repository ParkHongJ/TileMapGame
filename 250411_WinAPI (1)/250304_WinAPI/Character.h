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

	float					frameTime;
	POINT				 currFrameInd;
	POINT			  currMaxFrameInd;
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


	bool isOnPet;
	
		               
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
	void SetAnimationRange(PlayerState state);

	void SetFrameTime(float frameTime) { this->frameTime = frameTime; }

	

	void PlayAnimation(unsigned int stateClassNum, unsigned int subState );
	void ChangeState(CharacterState* newState);
	
	
	

	void HandleInput(PlayerState prevState, float TimeDelta);
	bool PressAnyKey();


	float GetVelocitySize();
	float GetYVelocity();



	void Move(int dirX, float TimeDelta);

	void LookUp(float TimeDelta);
	void LookDown(float TimeDelta);
	






	




	Character() {};
	virtual ~Character() {};

};