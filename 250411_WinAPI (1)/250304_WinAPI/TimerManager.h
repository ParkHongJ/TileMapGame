#pragma once
#include "config.h"
#include "Singleton.h"

class Timer;
class TimerManager : public Singleton<TimerManager>
{
private:
	Timer* timer;
	wchar_t szText[128];

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	float GetDeltaTime();

};

