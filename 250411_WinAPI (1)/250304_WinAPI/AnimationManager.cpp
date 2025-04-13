#include "AnimationManager.h"
#include "Animation.h"

AnimationManager::AnimationManager()
{
}

HRESULT AnimationManager::Init()
{
	return S_OK;
}

void AnimationManager::Update(float TimeDelta)
{
	Animations[CurAnimKey]->Update(TimeDelta);

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		Animations[CurAnimKey]->PlayEvent();
	}

}

void AnimationManager::Render(HDC hdc)
{
	Animations[CurAnimKey]->Render(hdc);
}

void AnimationManager::Release()
{
}

void AnimationManager::RegisterAnimation(string _Key, Animation* _Anim, bool _IsDefault)
{
	Animations.emplace(_Key, _Anim);

	if (true == _IsDefault)
	{
		CurAnimKey = DefaultAnimKey =  _Key;
	}
}


