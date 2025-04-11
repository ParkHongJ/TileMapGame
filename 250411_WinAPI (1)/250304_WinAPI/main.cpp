// main.cpp

#include "config.h"
#include "MainGame.h"

HINSTANCE g_hInstance;	// 프로그램 인스턴스 핸들
HWND g_hWnd;
LPCWSTR g_lpszClassName = (LPCWSTR)TEXT("윈도우 API 사용하기");
MainGame g_mainGame;
POINT g_ptMouse;	// 마우스 좌표

// Init

RECT GetRect(int left, int top, int width, int height);
RECT GetRectAtCenter(int x, int y, int width, int height);

// Render
void RenderStar(HDC hdc, int posX, int posY);
void RenderRect(HDC hdc, int x, int y, int width, int height);
void RenderRectAtCenter(HDC hdc, int centerX, int centerY, int width, int height);
void RenderEllipse(HDC hdc, int x, int y, int width, int height);
void RenderEllipseAtCenter(HDC hdc, int centerX, int centerY, int width, int height);

// Collision
bool PointInRect(FPOINT ptMouse, RECT rc);	// ptInRect
bool RectInRect(RECT rc1, RECT rc2);
// Update
void UpdateRect(RECT& rc, FPOINT pt);


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
	LPSTR lpszCmdParam, int nCmdShow)
{
	g_hInstance = hInstance;

	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbClsExtra = 2068;
	wndClass.cbWndExtra = 2068;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(g_hInstance, IDC_ARROW);
	wndClass.hIcon = LoadIcon(g_hInstance, IDI_APPLICATION);
	wndClass.hInstance = g_hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = g_lpszClassName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHIELD));



	//// 윈도우를 생성하기 위한 데이터 셋팅
	//WNDCLASS wndClass;
	//wndClass.cbClsExtra = 0;
	//wndClass.cbWndExtra = 0;
	//wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//wndClass.hCursor = LoadCursor(g_hInstance, IDC_ARROW);
	//wndClass.hIcon = LoadIcon(g_hInstance, IDI_APPLICATION);
	//wndClass.hInstance = g_hInstance;
	//wndClass.lpfnWndProc = WndProc;		// 함수의 이름은 메모리주소이다.
	//wndClass.lpszClassName = g_lpszClassName;
	//wndClass.lpszMenuName = NULL;
	//wndClass.style = CS_HREDRAW | CS_VREDRAW;	// | : 비트연산자

	//RegisterClass(&wndClass);
	RegisterClassEx(&wndClass);

	//g_hWnd = CreateWindow(g_lpszClassName, g_lpszClassName,
	//	WS_OVERLAPPEDWINDOW, 50, 50, WINSIZE_X, WINSIZE_Y,
	//	NULL, NULL, g_hInstance, NULL);

	RECT rcWindowSize = { 0, 0, WINSIZE_X, WINSIZE_Y };
	AdjustWindowRect(&rcWindowSize, WS_OVERLAPPEDWINDOW, FALSE);
	int width = rcWindowSize.right - rcWindowSize.left;
	int height = rcWindowSize.bottom - rcWindowSize.top;

	g_hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, g_lpszClassName, g_lpszClassName,
		WS_OVERLAPPEDWINDOW, 50, 50, width, height,
		NULL, NULL, g_hInstance, NULL);

	ShowWindow(g_hWnd, nCmdShow);

	TimerManager::GetInstance()->Init();
	g_mainGame.Init();

	MSG message;
	while (true)
	{
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TimerManager::GetInstance()->Update();
			g_mainGame.Update();
			g_mainGame.Render();
		}
	}

	//while (GetMessage(&message, 0, 0, 0))
	//{
	//	TranslateMessage(&message);
	//	DispatchMessage(&message);
	//}

	g_mainGame.Release();
	TimerManager::GetInstance()->Release();

	return message.wParam;
}

LRESULT WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return g_mainGame.MainProc(hWnd, iMessage, wParam, lParam);
}
