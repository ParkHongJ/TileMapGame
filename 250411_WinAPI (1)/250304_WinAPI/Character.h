#pragma once
#include "config.h"
#include "GameObject.h"
class Character : public GameObject
{

private:

	PlayerState					state;

	Image*				  playerImage;

	FPOINT						  dir;
	FPOINT				     velocity;

	float					frameTime;
	POINT				 currFrameInd;
	POINT			  currMaxFrameInd;
	FrameInfo		    currFrameInfo;
	FrameInfo           jumpFrameInfo;
	FrameInfo		  attackFrameInfo;

	float						speed;
	float				  attackSpeed;
	float				   attackRate;

	bool					   isFlip;
	bool                      isInAir;
	bool                  isAttacking;
		               
	bool                  jumpPressed;
	bool                attackPressed;

	bool			   isLookUpPaused;
	bool			 isLookDownPaused;





public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	void SetAnimationRange(PlayerState state);
	bool ShouldResetAnimation(PlayerState prevState, PlayerState newState);
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;


	void HandleInput(PlayerState prevState, float TimeDelta);
	bool PressAnyKey();

	void UpdateIdle();
	void UpdateMove(float TimeDelta);
	void UpdateLookUp(float TimeDelta);
	void UpdateLookDown(float TimeDelta);
	void UpdateLookDownMove(float TimeDelta);
	void UpdateLookDownMoveStop(float TimeDelta);
	void UpdateClimb(float TimeDelta);
	void UpdateAttack(float TimeDelta);
	void UpdateHang(float TimeDelta);
	void UpdateHurt(float TimeDelta);
	void UpdateDie(float TimeDelta);
	void UpdateThrow(float TimeDelta);
	void UpdateHold(float TimeDelta);
	void UpdatePush(float TimeDelta);
	void UpdateExit(float TimeDelta);



	const TCHAR* PlayerStateToString(PlayerState state);

	Character() {};
	virtual ~Character() {};

};