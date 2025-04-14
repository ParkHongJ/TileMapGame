#include "HyoScene.h"
#include "ImageManager.h"
#include "Image.h"
#include "HyoCharacter.h"
#include "CommonFunction.h"

HRESULT HyoScene::Init(ID2D1HwndRenderTarget* renderTarget)
{

	background = ImageManager::GetInstance()->FindImage("Hyo_BackGround");

	ImageManager::GetInstance()->AddImage(
		"Character_Iris", L"Image/char_iris.png", renderTarget);
	/*background = new Image();

	if (FAILED(background->Init(TEXT("Image/bg_cave.bmp"), WINSIZE_X, WINSIZE_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("Image/bg_cave.bmp 생성 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}*/

	// 캐릭터 
	
	playerIris = new HyoCharacter();
	playerIris->Init();

	return S_OK;
}

void HyoScene::Release()
{
}

void HyoScene::Update(float TimeDelta)
{
}

void HyoScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
	background->Render(renderTarget, 0, 0);
	if (playerIris)
	{
		playerIris->Render(renderTarget);
	}
}
