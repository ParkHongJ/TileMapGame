#pragma once
#include "config.h"
#include "GameObject.h"
class Character : public GameObject
{

private:

	PlayerState state;

	Image* playerImage;
	//Image* weaponImage;

	FPOINT	dir;

	POINT currFrameInd;

	CurrFrameInfo currFrameInfo;

	POINT currMaxFrameInd;
	float frameTime;

	float speed;
	float attackSpeed;
	float attackRate;

	bool isFlip;
	bool isFalling;
	bool isAttakcing;

	bool jumpPressed;
	bool attackPressed;

	bool isLookUpPaused;





public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(HDC hdc) override;


	void HandleInput(float TimeDelta);

	void UpdateIdle();
	void UpdateMove(float TimeDelta);
	void UpdateLookUp(float TimeDelta);
	void UpdateLookDown(float TimeDelta);
	void UpdateJump(float TimeDelta);
	void UpdateFall(float TimeDelta);
	void UpdateClimb(float TimeDelta);
	void UpdateAttack(float TimeDelta);
	void UpdateCrouch(float TimeDelta);
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