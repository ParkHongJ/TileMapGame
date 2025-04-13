#include "TestAnimationObject.h"
#include "AnimationManager.h"

HRESULT TestAnimationObject::Init()
{
	Animation* Anim = new Animation();
	AnimManager = new AnimationManager();

	AnimManager->RegisterAnimation("테스트", Anim, true);
	AnimManager->RegisterAnimationEvent("테스트", "Guard", this, &TestAnimationObject::Guard);

	Count = 0;
	return S_OK;
}

void TestAnimationObject::Update(float TimeDelta) 
{
	AnimManager->Update(TimeDelta);
}

void TestAnimationObject::Render(HDC hdc)
{
	AnimManager->Render(hdc);
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
	++Count;
}

void TestAnimationObject::Hit()
{
	int i = 5;
}
