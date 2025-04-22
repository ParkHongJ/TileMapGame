#pragma once
#include "GameObject.h"
#include "Animation.h"

class AnimationManager: public GameObject
{
public:
	AnimationManager();
	virtual ~AnimationManager() {};

	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;

	bool RegisterAnimation(string _Key, Animation* _Anim, bool _IsDefault = false);

	template<typename T, typename Ret, typename... Args>
	inline void RegisterAnimationEvent(string _Key, string _EventName, float _EventTime, T* _Owner, Ret(T::*_MemFunc)(Args...), Args&&... _Args)
	{
		Animations[_Key]->RegisterEvent(_EventName, _EventTime, _Owner, _MemFunc, std::forward<Args>(_Args)...);
	}

	bool ChangeAnimation(string _Key);

private:

	string CurAnimKey;
	string PreAnimKey;
	string DefaultAnimKey;

	map<string, Animation*> Animations;
	map<string, Animation*> AnimationMontages; // 필요한가
};


