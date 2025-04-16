#include "HyoScene.h"
#include "ImageManager.h"
#include "Image.h"
#include "HyoCharacter.h"
#include "CommonFunction.h"
#include "CameraManager.h"

HRESULT HyoScene::Init(ID2D1HwndRenderTarget* renderTarget)
{

	background = ImageManager::GetInstance()->FindImage("Hyo_BackGround");

	ImageManager::GetInstance()->AddImage(
		"testBackGround", L"Image/testBackGround.png", renderTarget);

	ImageManager::GetInstance()->AddImage(
		"testCamera", L"Image/textCamera.png", renderTarget);

	// 캐릭터 
	testCamera = new HyoCharacter();
	testCamera->Init();

	CameraManager::GetInstance()->Init();
	CameraManager::GetInstance()->SetMapWidth(testCamera->GetBackGroundWidth());
	CameraManager::GetInstance()->SetMapHeight(testCamera->GetBackGroundHeight());

	cameraPos = CameraManager::GetInstance()->GetPos();

	return S_OK;
}

void HyoScene::Release()
{
}

void HyoScene::Update(float TimeDelta)
{
	testCamera->Update(TimeDelta);
	CameraManager::GetInstance()->Update(testCamera->GetPos(), TimeDelta);
	// camera -> Update(testCamera->GetPos(), TimeDelta);	// 왜 const를 사용해야하는 지

}    

void HyoScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (testCamera)
	{
		testCamera->Render(renderTarget);
	}
}
