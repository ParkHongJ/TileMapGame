#include "Animation.h"
#include "Image.h"

Animation::Animation() : 
	MaxFrameX(1), MaxFrameY(1), FrameWidth(1), FrameHeight(1),
	CurFrame(0), CurFrameX(0), CurFrameY(0), FrameSpeed(1.f), ElapseTime(0.f),
	IsFlip(false), IsEndAnimation(false), IsStayMaxFrame(false), AnimDir(AnimationDir::DIR_ORIGINAL)
{

}

HRESULT Animation::Init()
{

	return S_OK;
}

void Animation::Update(float TimeDelta)
{
	ElapseTime += FrameSpeed * TimeDelta;
	CurFrame = (int)ElapseTime;
	CurFrameY = CurFrame / MaxFrameX;
	CurFrameX = CurFrame % MaxFrameX;

	if (CurFrameY >= MaxFrameY || CurFrameX >= MaxFrameX)
	{
		ElapseTime = 0.f;
		CurFrameX = CurFrameY = CurFrame = 0;
	}
}

bool Animation::PlayEvent()
{
	if (!Events.empty())
	{
		Events.begin()->second();
		return true;
	}

	return false;
}

bool Animation::PlayEvent(string _Key)
{
	auto iter = Events.find(_Key);
	if (Events.end() != Events.find(_Key))
	{
		iter->second();
		return true;
	}

	return false;
}

void Animation::ResetAnimation()
{

}

void Animation::Render(HDC hdc)
{
	image->FrameRender(hdc, 0, 0, CurFrameX, CurFrameY, IsFlip);
}

void Animation::Release()
{
}
