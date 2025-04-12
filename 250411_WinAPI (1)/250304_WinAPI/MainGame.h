#pragma once
#include "config.h"
class Timer;
class Image;
class TilemapTool;
class MainGame
{
public:
	HRESULT Init();
	
	void Update();
	void Render();
	void Release();

	void FrameRender();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	MainGame();
	~MainGame();

	//

	// 엔터만쳐도 충돌이 났었는데.. 
	/// 이상하다.. 

private:
	PAINTSTRUCT ps;
	// 내..
	HDC hdc;

	wchar_t szText[128];
	Image* backBuffer;

	TilemapTool* tilemapTool;

	Timer* timer;

};

