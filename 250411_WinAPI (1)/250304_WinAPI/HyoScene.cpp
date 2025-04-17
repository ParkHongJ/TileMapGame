#include "HyoScene.h"
#include "ImageManager.h"
#include "Image.h"
#include "HyoCharacter.h"
#include "CommonFunction.h"
#include "CameraManager.h"
#include "TestHyoBackGround.h"

HRESULT HyoScene::Init(ID2D1HwndRenderTarget* renderTarget)
{

	background = ImageManager::GetInstance()->FindImage("Hyo_BackGround");

	ImageManager::GetInstance()->AddImage("testTile", L"Image/testTile.png", renderTarget);

	ImageManager::GetInstance()->AddImage(
		"testBackGround", L"Image/testBackGround.png", renderTarget);

	ImageManager::GetInstance()->AddImage(
		"testCamera", L"Image/textCamera.png", renderTarget);

	// 배경
	for (int i = 0; i < TILE_COUNT; i++)
	{
		for (int j = 0; j < TILE_COUNT; j++)
			testTile[i][j].Init(j,i);
	}

	// 캐릭터 
	testCamera = new HyoCharacter();
	testCamera->Init();

	CameraManager::GetInstance()->Init();
	/*CameraManager::GetInstance()->SetMapWidth(testCamera->GetBackGroundWidth());
	CameraManager::GetInstance()->SetMapHeight(testCamera->GetBackGroundHeight());*/

	cameraPos = CameraManager::GetInstance()->GetPos();
	canRender = false;
	return S_OK;
}

void HyoScene::Release()
{
}

void HyoScene::Update(float TimeDelta)
{
	for (int i = 0; i < TILE_COUNT; i++)
	{
		for (int j = 0; j < TILE_COUNT; j++)
		{
			testTile[i][j].Update(TimeDelta);
		}
	}

	testCamera->Update(TimeDelta);
	CameraManager::GetInstance()->Update(testCamera->GetPos(), TimeDelta);
	// camera -> Update(testCamera->GetPos(), TimeDelta);	// 왜 const를 사용해야하는 지
	                               
}    

void HyoScene::Render(ID2D1HwndRenderTarget* renderTarget)
{  
	Viewport viewTile = CameraManager::GetInstance()->GetInRect();

	for (int i = 0; i < TILE_COUNT; i++)
	{
		for (int j = 0; j < TILE_COUNT; j++)
		{
			/*if (testTile[i][j].GetRect().left < 0 || testTile[i][j].GetRect().right > WINSIZE_X
				|| testTile[i][j].GetRect().top < 0 || testTile[i][j].GetRect().bottom > WINSIZE_Y) continue;*/

			if (viewTile.left > testTile[i][j].GetRect().right || viewTile.right < testTile[i][j].GetRect().left
				|| viewTile.top > testTile[i][j].GetRect().bottom || viewTile.bottom < testTile[i][j].GetRect().top)
			{
				canRender = false;
			}
			else canRender = true;
			
			if(canRender)
				testTile[i][j].Render(renderTarget);
		}
	}

	if (testCamera)
	{
		testCamera->Render(renderTarget);
	}
}


