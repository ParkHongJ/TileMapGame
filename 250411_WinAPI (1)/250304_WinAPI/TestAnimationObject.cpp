#include "TestAnimationObject.h"
#include "AnimationManager.h"

HRESULT TestAnimationObject::Init()
{
	Animation* Test = new Animation();
	//Animation* Test = new Animation(this, &TestAnimationObject::Attack, 5);
	AnimManager = new AnimationManager();

	AnimManager->RegisterAnimation("테스트", Test);
	int i = 5;
	AnimManager->RegisterAnimationEvent("테스트", this, &TestAnimationObject::Guard);
	return S_OK;
}

void TestAnimationObject::Update(float TimeDelta) 
{
	AnimManager->Update(TimeDelta);
}

void TestAnimationObject::Render(HDC hdc)
{
}

void TestAnimationObject::Release()
{
	if (AnimManager)
	{
		AnimManager->Release();
		delete AnimManager;
		AnimManager = nullptr;
	}
}

void TestAnimationObject::Attack(int _Test)
{
	int i = 5;
}

void TestAnimationObject::Guard()
{
	int i = 5;
}

void TestAnimationObject::Hit()
{
	int i = 5;
}
