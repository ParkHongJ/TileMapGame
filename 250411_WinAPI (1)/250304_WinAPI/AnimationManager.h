#pragma once
#include "GameObject.h"
#include "Animation.h"

class AnimationManager: public GameObject
{
public:
	AnimationManager() {};
	virtual ~AnimationManager() {};

	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	void RegisterAnimation(string _Key, Animation* _Anim);

	template<typename T, typename Ret, typename... Args>
	inline void RegisterAnimationEvent(string _Key, T* _Owner, Ret(T::*_MemFunc)(Args...), Args&&... _Args)
	{
		Animations[_Key]->RegisterEvent(_Owner, _MemFunc, std::forward<Args>(_Args)...);
	}

private:
	map<string, Animation*> Animations;
};


