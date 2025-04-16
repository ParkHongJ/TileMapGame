#pragma once
#include "config.h"
#include "Singleton.h"

class Timer;
class TimerManager : public Singleton<TimerManager>
{
private:
	wchar_t szText[128];
	map<wstring, Timer*> timers;
public:
	void Init();
	void Release();
	void Update();
	void Update(const wstring& timerKey);
	void Render(ID2D1HwndRenderTarget* renderTarget);

	float GetDeltaTime(const wstring& timerKey);
	
};

