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





public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(HDC hdc) override;

	Character() {};
	virtual ~Character() {};

};