#pragma once
#include <functional>

using CallbackType = std::function<void()>;

class Image;
class GameObject;

class Animation
{
public:
	Animation() {};

	Animation(CallbackType&& _Callback) : Callback(_Callback)
	{
	}

	template<typename T, typename Ret, typename... Args>
	Animation(T* _Owner, Ret(T::* _MemFunc)(Args...), Args&&... _Args)
	{
		Callback = [_Owner, _MemFunc, _Args...]() { (_Owner->*_MemFunc)(_Args...); };
	}

	template<typename T, typename Ret, typename... Args>
	inline void RegisterEvent(T* _Owner, Ret(T::*_MemFunc)(Args...), Args&&... _Args)
	{
		Callback = [_Owner, _MemFunc, _Args...]() { (_Owner->*_MemFunc)(_Args...); };
	}

	void Action()
	{
		Callback();
	}

	//Animation() = default;
	virtual ~Animation() {};

	inline Image* GetImage() const { return Image; };
	inline void const SetImage(Image* _Image) { Image = _Image; };
private:
	Image* Image;
	CallbackType Callback;

	// 애니메이션 관련
	int maxFrameX;
	int maxFrameY;
	int frameWidth;
	int frameHeight;
	int currFrameX;
	int currFrameY;

public:






private:
	

};

