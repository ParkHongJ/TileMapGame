#include "BattleScene.h"
#include "Image.h"
#include "EnemyManager.h"
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

	enemyManager = new EnemyManager();
	enemyManager->Init();

	Sleep(3000);


	return S_OK;
}

void BattleScene::Release()
{
	if (enemyManager)
	{
		enemyManager->Release();
		delete enemyManager;
		enemyManager = nullptr;
	}

	if (backGround)
	{
		backGround->Release();
		delete backGround;
		backGround = nullptr;
	}
}

void BattleScene::Update()
{
	enemyManager->Update();
}

void BattleScene::Render(HDC hdc)
{
	backGround->Render(hdc);
	enemyManager->Render(hdc);
}
