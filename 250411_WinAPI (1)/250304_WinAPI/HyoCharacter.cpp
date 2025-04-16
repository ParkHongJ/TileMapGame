#include "HyoCharacter.h"
#include "Image.h"
#include "ImageManager.h"
#include "CommonFunction.h"
#include "KeyManager.h"

HRESULT HyoCharacter::Init()
{
	testBackGround = ImageManager::GetInstance()->FindImage("testBackGround");
	testCamera = ImageManager::GetInstance()->FindImage("testCamera");

	/*playerIris = new Image();

	if (FAILED(playerIris->Init(TEXT("Image/char_iris.bmp"), WINSIZE_X, WINSIZE_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("Image/char_iris.bmp 생성 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}*/

	SetPos({ WINSIZE_X / 2,WINSIZE_Y / 2 - 10 });
	mapSizeHeight = testBackGround->GetHeight();
	maptSizeWidth = testBackGround->GetWidth();
	dir = -1;
	backGroundPos = { WINSIZE_X / 2,WINSIZE_Y / 2 };
	state = PlayerState::IDLE;
	
	return S_OK;	
}

void HyoCharacter::Release()
{
	
}

void HyoCharacter::Update(float TimeDelta)
{
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_RIGHT))
	{
		Pos.x += 200 * TimeDelta;
	}
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_LEFT))
	{
		Pos.x -= 200 * TimeDelta;
	}
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_UP))
	{
		Pos.y -= 200 * TimeDelta;
	}
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_DOWN))
	{
		Pos.y += 200 * TimeDelta;
	}

	
}

void HyoCharacter::Render(ID2D1HwndRenderTarget* renderTarget)
{
	
	testCamera->Render(renderTarget, Pos.x, Pos.y);
	//playerIris->Render(renderTarget, Pos.x, Pos.y);

	//playerIris->Render(hdc);
}

void HyoCharacter::TestRender(ID2D1HwndRenderTarget* renderTarget, const FPOINT& cameraPos)
{
	if (testBackGround)
	{
		//testBackGround->Render(renderTarget, cameraPos.x, cameraPos.y);
		testBackGround->Render(renderTarget, backGroundPos.x + cameraPos.x, backGroundPos.y + cameraPos.y);
	}
	
	if (testCamera)
	{
		//testCamera->Render(renderTarget, cameraPos.x, cameraPos.y);
		testCamera->Render(renderTarget, Pos.x + cameraPos.x, Pos.y + cameraPos.y);
	}
	
	// testCamera->Render(renderTarget, 0, 0);
}
