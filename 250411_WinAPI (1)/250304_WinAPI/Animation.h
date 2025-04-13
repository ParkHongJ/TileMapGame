#pragma once
#include "GameObject.h"
#include <functional>

using CallbackType = std::function<void()>;

class Image;

enum class AnimationDir { DIR_ORIGINAL, DIR_REVERSE };

class Animation : public GameObject
{
public:
	Animation();

	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	Animation(string _Key, CallbackType&& _Callback)
	{
		Events[_Key] = _Callback;
	};

	template<typename T, typename Ret, typename... Args>
	Animation(string _Key, T* _Owner, Ret(T::* _MemFunc)(Args...), Args&&... _Args)
	{
		Events[_Key] = [_Owner, _MemFunc, _Args...]() { (_Owner->*_MemFunc)(_Args...); };
	}

	template<typename T, typename Ret, typename... Args>
	inline bool RegisterEvent(string _Key, T* _Owner, Ret(T::*_MemFunc)(Args...), Args&&... _Args)
	{
		bool IsExist = Events.end() != Events.find(_Key);
		if (false == IsExist)
		{
			Events[_Key] = [_Owner, _MemFunc, _Args...]() { (_Owner->*_MemFunc)(_Args...); };
		}
		return !IsExist;
	}

	template<typename T, typename Ret, typename... Args>
	inline bool ChangeEvent(string _Key, T* _Owner, Ret(T::* _MemFunc)(Args...), Args&&... _Args)
	{
		bool IsExist = Events.end() != Events.find(_Key);
		if (true == IsExist)
		{
			Events[_Key] = [_Owner, _MemFunc, _Args...]() { (_Owner->*_MemFunc)(_Args...); };
		}
		return IsExist;
	}

	bool PlayEvent();
	bool PlayEvent(string _Key);

	void ResetAnimation();

public:
	inline void SetImage(Image* _Image) { image = _Image; };
	inline Image* GetImage() const { return image; };

	virtual ~Animation() {};

private:
	Image* image;
	map<string, CallbackType> Events;

	// 애니메이션 관련

	int MaxFrameX;
	int MaxFrameY;
	int FrameWidth;
	int FrameHeight;

	int CurFrame;
	int CurFrameX;
	int CurFrameY;

	float FrameSpeed;
	float ElapseTime;

	bool IsFlip;
	bool IsEndAnimation;
	bool IsStayMaxFrame;

	AnimationDir AnimDir;
};

