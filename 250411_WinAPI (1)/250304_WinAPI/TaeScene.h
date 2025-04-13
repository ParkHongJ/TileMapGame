#pragma once
#include "Scene.h"
#include "GameObject.h"


#define ANIMATION_FRAME_TIME 0.1f

enum class PlayerState {
	IDLE,
	MOVE,
	JUMP,
};



class Image;

typedef struct tagCurrFrameInfo
{
	POINT startFrame;
	POINT endFrame;


} CurrFrameInfo;


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

class TaeScene : public Scene
{
private:

	Image* backGround;


	//temp

	Character* yellow;



public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(HDC hdc) override;

	TaeScene() {};
	virtual ~TaeScene() {};

};

