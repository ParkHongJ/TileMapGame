#include "HyoScene.h"
#include "ImageManager.h"
#include "Image.h"
#include "HyoCharacter.h"
#include "CommonFunction.h"
#include "Camera.h"

HRESULT HyoScene::Init(ID2D1HwndRenderTarget* renderTarget)
{

	background = ImageManager::GetInstance()->FindImage("Hyo_BackGround");

	ImageManager::GetInstance()->AddImage(
		"testBackGround", L"Image/testBackGround.png", renderTarget);

	ImageManager::GetInstance()->AddImage(
		"testCamera", L"Image/textCamera.png", renderTarget);
	/*background = new Image();

	if (FAILED(background->Init(TEXT("Image/bg_cave.bmp"), WINSIZE_X, WINSIZE_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("Image/bg_cave.bmp 생성 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}*/

	// 캐릭터 
	/*testBackGround = new HyoCharacter();
	testBackGround->Init();*/

	testCamera = new HyoCharacter();
	testCamera->Init();

	camera = new Camera();
	camera->Init();

	camera->SetMapWidth(testCamera->GetBackGroundWidth());
	camera->SetMapHeight(testCamera->GetBackGroundHeight());

	return S_OK;
}

void HyoScene::Release()
{
}

void HyoScene::Update(float TimeDelta)
{
	testCamera->Update(TimeDelta);
	camera -> Update(testCamera->GetPos(), TimeDelta);	// 왜 const를 사용해야하는 지

}    

void HyoScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
	//background->Render(renderTarget, 0, 0);	// 배경에도 카메라 위치 세팅 
	/*if (testBackGround)
	{
		testBackGround->TestRender(renderTarget, camera->GetPos());
	}
*/

	if (testCamera)
	{
		testCamera->TestRender(renderTarget,camera->GetPos());
	}
}
