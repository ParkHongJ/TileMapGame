//#pragma once
//#include "config.h"
//class Timer;
//class Image;
//class TilemapTool;
//class MainGame
//{
//private:
//	HDC hdc;
//	PAINTSTRUCT ps;
//	wchar_t szText[128];
//
//	Image* backBuffer;
//
//	TilemapTool* tilemapTool;
//
//	Timer* timer;
//
//public:
//	HRESULT Init();
//	void Release();
//	void Update();
//	void Render();
//
//	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
//
//	MainGame();
//	~MainGame();
//};

#pragma once
#include "config.h"
class Timer;
class Image;
class TilemapTool;
class MainGame
{
public:
	HRESULT Init() override;
	
	void Update() override;
	void Render();
	void Release();

	void FrameRender();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	MainGame();
	~MainGame();

	//

	// ���͸��ĵ� �浹�� �����µ�.. 
	/// �̻��ϴ�.. 

private:
	PAINTSTRUCT ps;
	// ��..
	HDC hdc;

	wchar_t szText[128];
	Image* backBuffer;

	TilemapTool* tilemapTool;

	Timer* timer;

};

