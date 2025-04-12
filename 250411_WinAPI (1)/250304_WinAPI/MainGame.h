#pragma once
#include "config.h"
class Timer;
class Image;
class TilemapTool;
class MainGame
{
private:
	HDC hdc;
	PAINTSTRUCT ps;
	wchar_t szText[128];

	Image* backBuffer;

	TilemapTool* tilemapTool;

	Timer* timer;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render();

	// 충돌내긔

	// 뜌어어어
	
	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	MainGame();
	~MainGame();

	///
};

