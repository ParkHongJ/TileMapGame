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
	void LateUpdate();
	void Render();
	void Release();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	MainGame();
	~MainGame();

private:

	HRESULT InitD2D();

	void BeginDraw();
	void Draw();
	void EndDraw();

private:
	//Legacy
	/*PAINTSTRUCT ps;
	HDC hdc;*/

	// D2D ฐüทร
	ComPtr<ID2D1Factory> m_pFactory;
	ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;
	ComPtr<ID2D1SolidColorBrush> m_pBrush;

	wchar_t szText[128];
	Image* backBuffer;

	TilemapTool* tilemapTool;

	Timer* timer;
};

