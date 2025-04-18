#pragma once
#include "GameObject.h"
class AnimationManager;
class PlayerStatus;
class Item;

class TestAnimationObject : public GameObject
{
public:
	TestAnimationObject();
	virtual ~TestAnimationObject() {};

	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;

	virtual void Detect(GameObject* obj) override;
private:
	void Attack(int _Test);
	void Guard();
	void Hit();

private:
	bool testAbleHold = false;
	bool testHold = false;
	AnimationManager* AnimManager;
	class PlayerStatus* status;
	int Count;
	class Image* image;

	Item* HoldItem;
	Item* BackItem;

	FPOINT itemOffsetPos;
};

