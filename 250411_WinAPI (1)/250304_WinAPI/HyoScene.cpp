#include "HyoScene.h"
#include "ImageManager.h"
#include "Image.h"
#include "CommonFunction.h"

HRESULT HyoScene::Init()
{
	ImageManager::GetInstance()->AddImage("Hyo_BackGround", TEXT("Image/bg_cave.bmp"),512,512);

	background = new Image();

	if (FAILED(background->Init(TEXT("Image/bg_cave.bmp"), WINSIZE_X, WINSIZE_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("Image/bg_cave.bmp ���� ����"), TEXT("���"), MB_OK);
		return E_FAIL;
	}

	// ĳ���� 

	return S_OK;
}

void HyoScene::Release()
{
}

void HyoScene::Update(float TimeDelta)
{
}

void HyoScene::Render(HDC hdc)
{
	background->Render(hdc);
}
