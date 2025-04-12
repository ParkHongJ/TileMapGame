#pragma once
#include "config.h"
class Timer;
class Image;
class TilemapTool;
class MainGame
{
private:
	PAINTSTRUCT ps;
	// ��..
	HDC hdc;

	wchar_t szText[128];
	Image* backBuffer;

	TilemapTool* tilemapTool;

	Timer* timer;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	MainGame();
	~MainGame();

	//

	// ���͸��ĵ� �浹�� �����µ�.. 
	/// �̻��ϴ�.. 
};

