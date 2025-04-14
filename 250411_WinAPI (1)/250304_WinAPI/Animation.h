#pragma once
#include "GameObject.h"
#include <functional>

using CallbackType = std::function<void()>;

class Image;

enum class AnimationDir { DIR_ORIGINAL, DIR_REVERSE };

class Animation
{
public:
	Animation();
	Animation(Image* _Image, bool _IsLoop = true);

	virtual HRESULT Init();
	virtual bool Update(float TimeDelta);
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);
	virtual void Release();

	Animation(string _Key, CallbackType&& _Callback)
	{
		Events[_Key] = _Callback;
	};

	template<typename T, typename Ret, typename... Args>
	Animation(string _Key, float _Time, T* _Owner, Ret(T::* _MemFunc)(Args...), Args&&... _Args)
	{
		Events[_Key] = [_Owner, _MemFunc, _Args...]() { (_Owner->*_MemFunc)(_Args...); };
		EventTimes[_Time] = _Key;
	}

	template<typename T, typename Ret, typename... Args>
	inline bool RegisterEvent(string _Key, float _Time, T* _Owner, Ret(T::*_MemFunc)(Args...), Args&&... _Args)
	{
		bool IsExist = Events.end() != Events.find(_Key);
		if (false == IsExist)
		{
			Events[_Key] = [_Owner, _MemFunc, _Args...]() { (_Owner->*_MemFunc)(_Args...); };
			EventTimes[_Time] = _Key;
		}
		return !IsExist;
	}

	template<typename T, typename Ret, typename... Args>
	inline bool ChangeEvent(string _Key, float _Time, T* _Owner, Ret(T::* _MemFunc)(Args...), Args&&... _Args)
	{
		bool IsExist = Events.end() != Events.find(_Key);
		if (true == IsExist)
		{
			Events[_Key] = [_Owner, _MemFunc, _Args...]() { (_Owner->*_MemFunc)(_Args...); };
			EventTimes[_Time] = _Key;
		}
		return IsExist;
	}

	bool PlayEvent();
	bool PlayEvent(string _Key);

	void ResetAnimationState();
	void UpdateEvent();

public:
	inline void SetImage(Image* _Image) { image = _Image; };
	inline Image* GetImage() const { return image; };

	virtual ~Animation() {};

private:
	Image* image;
	map<string, CallbackType> Events;
	map<float, string> EventTimes;

	// 애니메이션 관련

	int MaxFrameX;
	int MaxFrameY;
	int FrameWidth;
	int FrameHeight;

	int CurFrame;
	int CurFrameX;
	int CurFrameY;

	float FrameSpeed;
	float CurElapseTime;
	float PreElapseTime;

	bool IsFlip;
	bool IsLoop;
	bool IsEndAnimation;
	bool IsStayMaxFrame;

	AnimationDir AnimDir;
};

