#include "pch.h"
#include "MainGame.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Timer.h"
#include "BattleScene.h"
#include "TilemapTool.h"
#include "AstarScene.h"
#include "LoadingScene.h"
#include "CollisionManager.h"
#include "PlayerUI.h"
#include "HongScene.h"
#include "HyoScene.h"
#include "TaeScene.h"
#include "JinScene.h"
#include "YongScene.h"
#include "StartScene.h"
#include "PlayerSelectScene.h"
#include "GameScene.h"
#include "ObjectRegister.h"
#include "ParticleManager.h"
#include "GameManager.h"
#include "ImageManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"


HRESULT MainGame::Init()
{
	srand(time(NULL));

	if (FAILED(InitD2D()))
		return E_FAIL;

	GameManager::GetInstance()->CreateCaveRendertarget(m_pIntermediateRT);
	GameManager::GetInstance()->LoadScenes("caveScene");
	GameManager::GetInstance()->LoadScenes("hongScene");

	ImageManager::GetInstance()->Init();
	KeyManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init(m_pRenderTarget.Get());
	CollisionManager::GetInstance()->Init();
	ParticleManager::GetInstance()->Init();

	//SceneManager::GetInstance()->AddScene("A*�˰���", new AstarScene());
	//SceneManager::GetInstance()->AddScene("������_1", new BattleScene());

	SceneManager::GetInstance()->AddScene("ȫ��", new HongScene());
	SceneManager::GetInstance()->AddScene("ȿ��", new HyoScene());
	SceneManager::GetInstance()->AddScene("����", new JinScene());
	SceneManager::GetInstance()->AddScene("����", new StartScene());
	SceneManager::GetInstance()->AddScene("����", new PlayerSelectScene());
	SceneManager::GetInstance()->AddScene("�ؿ�", new YongScene());
	SceneManager::GetInstance()->AddScene("����", new GameScene());
	SceneManager::GetInstance()->AddScene("playerUI", new playerUI());

	//SceneManager::GetInstance()->AddLoadingScene("�ε�_1", new LoadingScene());


	backBuffer = ImageManager::GetInstance()->AddImage("BackBuffer", L"Textures/back.png", m_pRenderTarget.Get());

	ImageManager::GetInstance()->AddImage(
		"Hyo_BackGround", L"Image/bg_cave.bmp", m_pRenderTarget.Get());

	ImageManager::GetInstance()->AddImage(
		"Tae_Player", TEXT("Textures/char_yellow.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_black", TEXT("Textures/char_black.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_blue", TEXT("Textures/char_blue.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_cerulean", TEXT("Textures/char_cerulean.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_cinnabar", TEXT("Textures/char_cinnabar.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_cyan", TEXT("Textures/char_cyan.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_eggchild", TEXT("Textures/char_eggchild.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_gold", TEXT("Textures/char_gold.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_gray", TEXT("Textures/char_gray.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_green", TEXT("Textures/char_green.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_hired", TEXT("Textures/char_hired.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_iris", TEXT("Textures/char_iris.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_khaki", TEXT("Textures/char_khaki.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_lemon", TEXT("Textures/char_lemon.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_lime", TEXT("Textures/char_lime.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_magenta", TEXT("Textures/char_magenta.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_olive", TEXT("Textures/char_olive.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_orange", TEXT("Textures/char_orange.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_pink", TEXT("Textures/char_pink.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_red", TEXT("Textures/char_red.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_violet", TEXT("Textures/char_violet.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_white", TEXT("Textures/char_white.png"), 16, 16, m_pRenderTarget.Get());
	ImageManager::GetInstance()->AddImage("char_yellow", TEXT("Textures/char_yellow.png"), 16, 16, m_pRenderTarget.Get());



	//SceneManager::GetInstance()->ChangeScene("ȿ��");
	//SceneManager::GetInstance()->ChangeScene("����");
	//SceneManager::GetInstance()->ChangeScene("�ؿ�");

	SceneManager::GetInstance()->ChangeScene("����");

	//Legacy
	//hdc = GetDC(g_hWnd);


	/*backBuffer = new Image();
	if (FAILED(backBuffer->Init(TILEMAPTOOL_X, TILEMAPTOOL_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("����� ���� ����"), TEXT("���"), MB_OK);
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
	ObjectManager::GetInstance()->ReleaseInstance();
	ParticleManager::GetInstance()->Release();
	ParticleManager::GetInstance()->ReleaseInstance();
	CollisionManager::GetInstance()->ReleaseInstance();

	CoUninitialize(); // ���α׷� ���� �� ����
}

void MainGame::Update()
{
	TimerManager::GetInstance()->Update(L"60Frame");
	float deltaTime = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
	SceneManager::GetInstance()->Update(deltaTime);
	ObjectManager::GetInstance()->Update(deltaTime);
	CollisionManager::GetInstance()->Update(deltaTime);
	ParticleManager::GetInstance()->Update(deltaTime);

	if (false) // �� ����
	{
		fadeState = (lastWasFadeOut ? FadeState::FadeIn : FadeState::FadeOut);
		fadeT = 0.0f;
		lastWasFadeOut = !lastWasFadeOut;
	}

	if (fadeState != FadeState::None)
	{
		fadeT += deltaTime * fadeSpeed;
		if (fadeT >= 1.0f)
		{
			fadeT = 1.0f;
			fadeState = FadeState::None;
		}
	}
}

void MainGame::LateUpdate()
{
	float deltaTime = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
	//CollisionManager::GetInstance()->LateUpdate(deltaTime);
	ObjectManager::GetInstance()->LateUpdate(deltaTime);
	GameManager::GetInstance()->UpdateNavMesh();
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
			//SceneManager::GetInstance()->ChangeScene("������_1");
			break;
		case 'd': case 'D':
			SceneManager::GetInstance()->ChangeScene("�ؿ�");
			break;
		case 'w': case 'W':
			//SceneManager::GetInstance()->ChangeScene("playerUI");
			break;

		case '1':
			//SceneManager::GetInstance()->ChangeScene("����");
			break;
		case '3':
			//SceneManager::GetInstance()->ChangeScene("ȫ��");

			GameManager::GetInstance()->TravelToNextScene();
			break;
		case '4':
			SceneManager::GetInstance()->ChangeScene("ȿ��");
			break;

		case VK_RETURN:
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
	// Factory ����
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory),
		nullptr,
		reinterpret_cast<void**>(m_pFactory.GetAddressOf())
	);

	if (FAILED(hr))
		return E_FAIL;

	// RenderTarget ����
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRTProps =
		D2D1::HwndRenderTargetProperties(g_hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));

	m_pFactory->CreateHwndRenderTarget(rtProps, hwndRTProps, &m_pRenderTarget);

	// [2] �߰� ����Ÿ�� ���� (�޸𸮿� ��Ʈ�� ���)
	m_pRenderTarget->CreateCompatibleRenderTarget(
		D2D1::SizeF(WINSIZE_X, WINSIZE_Y),
		&m_pIntermediateRT
	);

	// �귯�� (��: �⺻ ��)
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
			MessageBox(nullptr, L"DWriteCreateFactory ����", L"Error", MB_OK);
		}

		GdwriteFactory->CreateTextFormat(
			L"���� ���",                // Font name
			nullptr,                    // Font collection
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20.0f,                      // Font size
			L"ko-kr",                   // Locale
			&GtextFormat
		);
	}

	D2D1_SIZE_U size = D2D1::SizeU(WINSIZE_X, WINSIZE_Y);
	D2D1_BITMAP_PROPERTIES props = {};
	props.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
	props.dpiX = 96.0f;
	props.dpiY = 96.0f;

	std::vector<DWORD> pixels(WINSIZE_X * WINSIZE_Y, 0xFF0000FF); // �Ķ��� RGBA

	//m_pRenderTarget->CreateBitmap(
	//	size,
	//	pixels.data(),
	//	WINSIZE_X * 4, // pitch
	//	&props,
	//	&blueTexture
	//);

	return S_OK;
}

void MainGame::BeginDraw()
{
	ID2D1RenderTarget* pRT1 = m_pIntermediateRT;

	pRT1->BeginDraw();

	pRT1->Clear(D2D1::ColorF(D2D1::ColorF::DarkGray));


	ID2D1RenderTarget* caveRT = GameManager::GetInstance()->GetCaveRenderTarget();

	// caveRT�� ���� �̸� ������
	caveRT->BeginDraw();
	caveRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));  // ���

}

void MainGame::Draw()
{
	ID2D1RenderTarget* pRT1 = m_pIntermediateRT;


	SceneManager::GetInstance()->Render(pRT1);
	ObjectManager::GetInstance()->Render(pRT1);
	ParticleManager::GetInstance()->Render(pRT1);

	CollisionManager::GetInstance()->DebugRender(pRT1);
	TimerManager::GetInstance()->Render(pRT1);

}

void MainGame::EndDraw()
{
	//HRESULT hr = m_pRenderTarget->EndDraw();
	ID2D1RenderTarget* pRT1 = m_pIntermediateRT;
	
	HRESULT hr = pRT1->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		m_pRenderTarget.Reset();
		Init(); // �ٽ� ����
	}

	// 1. �߰� ����Ÿ���� ����� ��Ʈ������ ����
	ComPtr<ID2D1Bitmap> finalBitmap;
	ComPtr<ID2D1Bitmap> caveBitmap;
	HRESULT hrBitmap = m_pIntermediateRT->GetBitmap(&finalBitmap);

	if (SUCCEEDED(hrBitmap))
	{
		m_pRenderTarget->BeginDraw();

		float t = EaseInOut(fadeT);

		float scaleFinal, alphaFinal;
		float scaleBlue, alphaBlue;

		if (fadeState == FadeState::FadeOut || (fadeState == FadeState::None && lastWasFadeOut))
		{
			// �� final �� blue (������� ����)
			scaleFinal = 1.0f + 0.5f * t;   // Ŀ��
			alphaFinal = 1.0f - t;

			scaleBlue = 0.9f + 0.1f * t;   // Ŀ��
			alphaBlue = t;
		}
		else if (fadeState == FadeState::None && !lastWasFadeOut)
		{
			// ���� ���� (���� ��): �ƹ� ��ȭ ����
			scaleFinal = 1.0f;
			alphaFinal = 1.0f;

			scaleBlue = 1.0f;
			alphaBlue = 0.0f;
		}
		else
		{
			// �� blue �� final (������� ����)
			scaleFinal = 1.5f - 0.5f * t;   // �۾���
			alphaFinal = t;

			scaleBlue = 1.0f - 0.1f * t;   // �۾���
			alphaBlue = 1.0f - t;
		}


		D2D1_POINT_2F center = D2D1::Point2F(WINSIZE_X * 0.5f, WINSIZE_Y * 0.5f);
		{
			D2D1::Matrix3x2F transform =
				D2D1::Matrix3x2F::Translation(-center.x, -center.y) *
				D2D1::Matrix3x2F::Scale(scaleFinal, scaleFinal) *
				D2D1::Matrix3x2F::Translation(center.x, center.y);

			m_pRenderTarget->SetTransform(transform);

			m_pRenderTarget->DrawBitmap(
				finalBitmap.Get(),
				D2D1::RectF(0, 0, WINSIZE_X, WINSIZE_Y),
				alphaFinal,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
			);
		}

		{
			D2D1::Matrix3x2F transform =
				D2D1::Matrix3x2F::Translation(-center.x, -center.y) *
				D2D1::Matrix3x2F::Scale(scaleBlue, scaleBlue) *
				D2D1::Matrix3x2F::Translation(center.x, center.y);

			m_pRenderTarget->SetTransform(transform);

			GameManager::GetInstance()->GetCaveRenderTarget()->EndDraw();
			GameManager::GetInstance()->GetCaveRenderTarget()->GetBitmap(&caveBitmap);
			
			m_pRenderTarget->DrawBitmap(
				caveBitmap.Get(),
				D2D1::RectF(0, 0, WINSIZE_X, WINSIZE_Y),
				alphaBlue,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
			);
		}
		
		
		hr = m_pRenderTarget->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{
			m_pRenderTarget.Reset();
			Init();
		}
	}
}
