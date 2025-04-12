#include "BattleScene.h"
#include "Image.h"
#include "CommonFunction.h"

HRESULT BattleScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

	backGround = new Image();
	if (FAILED(backGround->Init(TEXT("Image/BackGround.bmp"), WINSIZE_X, WINSIZE_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("Image/backGround.bmp 생성 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}

	Sleep(3000);


	return S_OK;
}

void BattleScene::Release()
{

	if (backGround)
	{
		backGround->Release();
		delete backGround;
		backGround = nullptr;
	}
}

void BattleScene::Update(float TimeDelta)
{
}

void BattleScene::Render(HDC hdc)
{
	backGround->Render(hdc);
}
