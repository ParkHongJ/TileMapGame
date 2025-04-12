#pragma once
#include "config.h"
class Timer;
class Image;
class ImageManager;
class TilemapTool;
class MainGame
{
public:
	HRESULT Init() override;
	
	void Update() override;
	void Render();
	void Release();
	
	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	void FrameRender();

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

