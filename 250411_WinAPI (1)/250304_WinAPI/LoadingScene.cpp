#include "LoadingScene.h"
#include "Image.h"

HRESULT LoadingScene::Init()
{
	/*bg = ImageManager::GetInstance()->AddImage("·Îµù_1",
		L"image/loading.bmp", 852, 480);*/
	return S_OK;
}

void LoadingScene::Release()
{
}

void LoadingScene::Update(float TimeDelta)
{
}

void LoadingScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
	//if (bg)
		//bg->Render(hdc);
}
