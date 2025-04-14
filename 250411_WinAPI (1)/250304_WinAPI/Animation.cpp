#include "Animation.h"
#include "Image.h"

Animation::Animation() : 
	MaxFrameX(1), MaxFrameY(1), FrameWidth(1), FrameHeight(1),
	CurFrame(0), CurFrameX(0), CurFrameY(0), FrameSpeed(1.f), CurElapseTime(0.f), PreElapseTime(0.f),
	IsFlip(false), IsLoop(true), IsEndAnimation(false), IsStayMaxFrame(false), AnimDir(AnimationDir::DIR_ORIGINAL), image(nullptr)
{

}
Animation::Animation(Image* _Image, bool _IsLoop) :
	MaxFrameX(1), MaxFrameY(1), FrameWidth(1), FrameHeight(1),
	CurFrame(0), CurFrameX(0), CurFrameY(0), FrameSpeed(1.f), CurElapseTime(0.f), PreElapseTime(0.f),
	IsFlip(false), IsLoop(_IsLoop), IsEndAnimation(false), IsStayMaxFrame(false), AnimDir(AnimationDir::DIR_ORIGINAL), image(_Image)
{
	//MaxFrameX = _Image->GetMaxFrameX();
	//MaxFrameY = _Image->GetMaxFrameY();
	FrameSpeed = 10.f;
}

HRESULT Animation::Init()
{
	return S_OK;
}

bool Animation::Update(float TimeDelta)
{
	bool Ret = false;

	CurElapseTime += FrameSpeed * TimeDelta;
	CurFrame = (int)CurElapseTime;
	CurFrameY = CurFrame / MaxFrameX;
	CurFrameX = CurFrame % MaxFrameX;

	if (CurFrameY >= MaxFrameY || CurFrameX >= MaxFrameX)
	{
		ResetAnimationState();
		Ret = true;
	}

	UpdateEvent();

	PreElapseTime = CurElapseTime;

	return Ret && !IsLoop;
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

void Animation::ResetAnimationState()
{
	CurElapseTime = 0.f;
	CurFrameX = CurFrameY = CurFrame = 0;
}

void Animation::UpdateEvent()
{
	for (auto& iter : EventTimes)
	{
		if (iter.first > CurElapseTime)
		{
			break;
		}

		else if (iter.first <= CurElapseTime && iter.first >= PreElapseTime)
		{
			PlayEvent(iter.second);
		}
	}
}

void Animation::Render(ID2D1HwndRenderTarget* renderTarget)
{
	//image->FrameRender(renderTarget, 300, 300, CurFrame, CurFrameY/*, IsFlip*/);
	image->Render(renderTarget, 300, 300);
}

void Animation::Release()
{

}
