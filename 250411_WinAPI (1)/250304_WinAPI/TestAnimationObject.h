#pragma once
#include "GameObject.h"
class AnimationManager;

class TestAnimationObject : public GameObject
{
public:
	TestAnimationObject() {};
	virtual ~TestAnimationObject() {};

	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	void Attack(int _Test);
	void Guard();
	void Hit();

private:
	AnimationManager* AnimManager;
};

