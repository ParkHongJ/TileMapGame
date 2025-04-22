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
	float EaseInOut(float t)
	{
		return t < 0.5f ? 4.f * t * t * t : 1.f - powf(-2.f * t + 2.f, 3) / 2.f;
	}
	// D2D 관련
	ComPtr<ID2D1Factory> m_pFactory;
	ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;
	
	ID2D1BitmapRenderTarget* m_pIntermediateRT = nullptr;

	wchar_t szText[128];
	Image* backBuffer;

	TilemapTool* tilemapTool;

	enum class FadeState
	{
		None,
		FadeOut,   // final → blue
		FadeIn     // blue → final
	};

	FadeState fadeState = FadeState::None;
	float fadeT = 0.0f;
	float fadeSpeed = 1.0f;

	bool lastWasFadeOut = false;
	ID2D1Bitmap* blueTexture = nullptr; // 파란색 비트맵
};

