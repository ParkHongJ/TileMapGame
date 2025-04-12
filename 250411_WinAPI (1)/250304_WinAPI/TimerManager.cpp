#include "TimerManager.h"
#include "Timer.h"
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

void TimerManager::Render(HDC hdc)
{
	map<wstring, Timer*>::iterator iter = timers.find(L"60Frame");
	if (iter != timers.end())
	{
		wsprintf(szText, TEXT("FPS : %d"), iter->second->GetFPS());
		TextOut(hdc, WINSIZE_X - 130, 20, szText, wcslen(szText));
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
