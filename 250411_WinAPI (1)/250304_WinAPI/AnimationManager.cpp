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
	if (true == Animations[CurAnimKey]->Update(TimeDelta))
	{
		ChangeAnimation(PreAnimKey);
	}
}

void AnimationManager::Render(HDC hdc)
{
	Animations[CurAnimKey]->Render(hdc);
}

void AnimationManager::Release()
{
	for (auto& iter : Animations)
	{
		iter.second->Release();
		delete iter.second;
		iter.second = nullptr;
	}

	Animations.clear();

	for (auto& iter : AnimationMontages)
	{
		iter.second->Release();
		delete iter.second;
		iter.second = nullptr;
	}
	
	AnimationMontages.clear();
}

bool AnimationManager::RegisterAnimation(string _Key, Animation* _Anim, bool _IsDefault)
{
	bool IsExist = Animations.end() != Animations.find(_Key);

	if (false == IsExist)
	{
		Animations.emplace(_Key, _Anim);

		if (true == _IsDefault)
		{
			CurAnimKey = DefaultAnimKey = _Key;
		}
	}

	return IsExist;
}

bool AnimationManager::ChangeAnimation(string _Key)
{
	bool IsSameAnim = _Key == CurAnimKey;

	if (!IsSameAnim)
	{
		Animations[_Key]->ResetAnimationState();
		PreAnimKey = CurAnimKey;
		CurAnimKey = _Key;
	}
	
	return !IsSameAnim;
}


