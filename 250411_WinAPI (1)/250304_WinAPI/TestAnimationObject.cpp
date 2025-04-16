#include "TestAnimationObject.h"
#include "AnimationManager.h"
#include "PlayerStatus.h"

HRESULT TestAnimationObject::Init()
{
	AnimManager = new AnimationManager();

	Image* Temp = ImageManager::GetInstance()->FindImage("TestJunYongAttack");

	Animation* Anim = new Animation(Temp, false);
	AnimManager->RegisterAnimation("공격", Anim);
	AnimManager->RegisterAnimationEvent("공격", "Attack", 3.f, this, &TestAnimationObject::Attack, 123);

	Temp = ImageManager::GetInstance()->FindImage("TestJunyongWalk");
		/*ImageManager::GetInstance()->AddImage(
		"TestJunyongWalk", L"Image/TestJunyongWalk.bmp", 904, 113,
		8, 1, true, RGB(255, 0, 255));*/

	Anim = new Animation(Temp);
	AnimManager->RegisterAnimation("걷기", Anim, true);
	Count = 0;

	status = new PlayerStatus();

	return S_OK;
}

void TestAnimationObject::Update(float TimeDelta) 
{
	if (KeyManager::GetInstance()->IsOnceKeyDown('Q'))
	{
		//AnimManager->ChangeAnimation("공격");
		objectRenderId = RENDER_HOLD;
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		//AnimManager->ChangeAnimation("공격");
		objectRenderId = RENDER_BACKGROUND;
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('E'))
	{
		objectRenderId = RENDER_PLAYER;
		//AnimManager->ChangeAnimation("공격");
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('R'))
	{
		objectRenderId = RENDER_PLAYERCLIMB;
		//AnimManager->ChangeAnimation("공격");
	}


	AnimManager->Update(TimeDelta);
}

void TestAnimationObject::Render(ID2D1HwndRenderTarget* renderTarget)
{
	AnimManager->Render(renderTarget);
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
	int i = _Test;
	int j = 5;
}

void TestAnimationObject::Guard()
{
	++Count;
}

void TestAnimationObject::Hit()
{
	int i = 5;
}
