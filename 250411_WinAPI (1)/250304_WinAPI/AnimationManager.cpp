#include "AnimationManager.h"
#include "Animation.h"

HRESULT AnimationManager::Init()
{
	//Animation Temp = new Animation()
	//Animations.emplace()
	return S_OK;
}

void AnimationManager::Update(float TimeDelta)
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		Animations["Å×½ºÆ®"]->Action();
	}
}

void AnimationManager::Render(HDC hdc)
{
}

void AnimationManager::Release()
{
}

void AnimationManager::RegisterAnimation(string _Key, Animation* _Anim)
{
	Animations.emplace(_Key, _Anim);
}


