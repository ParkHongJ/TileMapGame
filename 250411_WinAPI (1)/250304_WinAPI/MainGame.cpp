#include "MainGame.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Timer.h"
#include "BattleScene.h"
#include "TilemapTool.h"
#include "AstarScene.h"
#include "LoadingScene.h"
#include "CollisionManager.h"
#include "GameOverUI.h"
#include "PlayerUI.h"
#include "HongScene.h"
#include "HyoScene.h"
#include "TaeScene.h"
#include "JinScene.h"
#include "YongScene.h"
#include "GameScene.h"
#include "ObjectRegister.h"


HRESULT MainGame::Init()
{
	srand(time(NULL));

	if (FAILED(InitD2D()))
		return E_FAIL;

	ImageManager::GetInstance()->Init();
	KeyManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init(m_pRenderTarget.Get());

	//SceneManager::GetInstance()->AddScene("A*알고리즘", new AstarScene());
	//SceneManager::GetInstance()->AddScene("전투씬_1", new BattleScene());

	SceneManager::GetInstance()->AddScene("홍준", new HongScene());
	SceneManager::GetInstance()->AddScene("효진", new HyoScene());
	SceneManager::GetInstance()->AddScene("진석", new JinScene());
	SceneManager::GetInstance()->AddScene("태관", new TaeScene());
	SceneManager::GetInstance()->AddScene("준용", new YongScene());
	SceneManager::GetInstance()->AddScene("게임", new GameScene());
	SceneManager::GetInstance()->AddScene("playerUI", new playerUI());
	SceneManager::GetInstance()->AddScene("GameOverUI", new GameOverUI());

	//SceneManager::GetInstance()->AddLoadingScene("로딩_1", new LoadingScene());


	backBuffer = ImageManager::GetInstance()->AddImage("BackBuffer", L"Textures/char_lemon.png", m_pRenderTarget.Get());

	ImageManager::GetInstance()->AddImage(
		"Hyo_BackGround", L"Image/bg_cave.bmp", m_pRenderTarget.Get());

	ImageManager::GetInstance()->AddImage(
		"Tae_Player", TEXT("Textures/char_yellow.png"), 16, 16, m_pRenderTarget.Get());

	//SceneManager::GetInstance()->ChangeScene("효진");
	SceneManager::GetInstance()->ChangeScene("게임");

	//Legacy
	//hdc = GetDC(g_hWnd);


	/*backBuffer = new Image();
	if (FAILED(backBuffer->Init(TILEMAPTOOL_X, TILEMAPTOOL_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("백버퍼 생성 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}*/

	return S_OK;
}

void MainGame::Release()
{
	/*if (backBuffer)
	{
		backBuffer->Release();
		delete backBuffer;
		backBuffer = nullptr;
	}*/

	//Legacy
	//ReleaseDC(g_hWnd, hdc);


	SceneManager::GetInstance()->Release();
	KeyManager::GetInstance()->Release();
	ImageManager::GetInstance()->Release();

	CoUninitialize(); // 프로그램 종료 시 정리
}

void MainGame::Update()
{
	TimerManager::GetInstance()->Update(L"60Frame");
	float deltaTime = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
	SceneManager::GetInstance()->Update(deltaTime);
	ObjectManager::GetInstance()->Update(deltaTime);

}

void MainGame::LateUpdate()
{
	float deltaTime = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
	ObjectManager::GetInstance()->LateUpdate(deltaTime);
	CollisionManager::GetInstance()->Update(deltaTime);
}

void MainGame::Render()
{
	BeginDraw();

	Draw();

	EndDraw();

}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'a': case 'A':
			//SceneManager::GetInstance()->ChangeScene("전투씬_1");
			break;
		case 'd': case 'D':
			SceneManager::GetInstance()->ChangeScene("준용");
			break;
		case 'w': case 'W':
			SceneManager::GetInstance()->ChangeScene("GameOverUI");
			break;

		//case '1':
		//	SceneManager::GetInstance()->ChangeScene("태관");
			break;
		case '3':
			SceneManager::GetInstance()->ChangeScene("홍준");
			break;
		case '4':
			SceneManager::GetInstance()->ChangeScene("효진");
			break;
		}

		break;
	case WM_LBUTTONDOWN:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);

		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

HRESULT MainGame::InitD2D()
{
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	// Factory 생성
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory),
		nullptr,
		reinterpret_cast<void**>(m_pFactory.GetAddressOf())
	);

	if (FAILED(hr))
		return E_FAIL;

	// RenderTarget 설정
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRTProps =
		D2D1::HwndRenderTargetProperties(g_hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));

	m_pFactory->CreateHwndRenderTarget(rtProps, hwndRTProps, &m_pRenderTarget);

	// 브러시 (예: 기본 색)
	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &GBrush);

	if (!GdwriteFactory)
	{
		HRESULT hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(GdwriteFactory.GetAddressOf())
		);

		if (FAILED(hr))
		{
			MessageBox(nullptr, L"DWriteCreateFactory 실패", L"Error", MB_OK);
		}

		GdwriteFactory->CreateTextFormat(
			L"맑은 고딕",                // Font name
			nullptr,                    // Font collection
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20.0f,                      // Font size
			L"ko-kr",                   // Locale
			&GtextFormat
		);
	}

	return S_OK;
}

void MainGame::BeginDraw()
{
	m_pRenderTarget->BeginDraw();

	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DarkGray));
}

void MainGame::Draw()
{
	//이게 맵이라고 생각해
	//backBuffer->Render(m_pRenderTarget.Get(), 0, 0, 0.5f, 0.5f, 0.f, 0.f);

	SceneManager::GetInstance()->Render(m_pRenderTarget.Get());
	ObjectManager::GetInstance()->Render(m_pRenderTarget.Get());
	// Legacy
	//// 백버퍼에 먼저 복사
	//HDC hBackBufferDC = backBuffer->GetMemDC();

	//SceneManager::GetInstance()->Render(hBackBufferDC);
	CollisionManager::GetInstance()->DebugRender(m_pRenderTarget.Get());
	TimerManager::GetInstance()->Render(m_pRenderTarget.Get());
	//wsprintf(szText, TEXT("Mouse X : %d, Y : %d"), g_ptMouse.x, g_ptMouse.y);
	//TextOut(hBackBufferDC, 20, 60, szText, wcslen(szText));

	//// 백버퍼에 있는 내용을 메인 hdc에 복사
	//backBuffer->Render(hdc);
}

void MainGame::EndDraw()
{
	HRESULT hr = m_pRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		m_pRenderTarget.Reset();
		Init(); // 다시 생성
	}
}
