#include "pch.h"
#include "TimerManager.h"
#include "Timer.h"
#include "CommonFunction.h"
void TimerManager::Init()
{
	timers.insert(make_pair(L"Default", new Timer()));
	timers.insert(make_pair(L"60Frame", new Timer()));

	for (auto& iter : timers)
	{
		iter.second->Init();
	}
}

void TimerManager::Release()
{	
	for (auto& iter : timers)
	{
		iter.second->Release();
		delete iter.second;
	}
	timers.clear();
}

void TimerManager::Update()
{
	for (auto& iter : timers)
	{
		iter.second->Tick();
	}
}

void TimerManager::Update(const wstring& timerKey)
{
	map<wstring, Timer*>::iterator iter = timers.find(timerKey);
	if (iter != timers.end())
	{
		iter->second->Tick();
	}
}

void TimerManager::Render(ID2D1RenderTarget* renderTarget)
{
	map<wstring, Timer*>::iterator iter = timers.find(L"60Frame");
	if (iter != timers.end())
	{
		
		wstring fps = L"FPS : " + to_wstring(iter->second->GetFPS());

		D2D1_RECT_F rect = { WINSIZE_X - 200, 50, WINSIZE_X - 200 + 100, 100 };
		DrawD2DText(renderTarget, fps.c_str(), rect.left, rect.top);
	}
}

float TimerManager::GetDeltaTime(const wstring& timerKey)
{
	map<wstring, Timer*>::iterator iter = timers.find(timerKey);
	if (iter == timers.end())
	{
		return 0.f;
	}
	return iter->second->GetDeltaTime();
}
