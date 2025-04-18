#include "pch.h"
#include "LoadingScene.h"
#include "Image.h"

HRESULT LoadingScene::Init(ID2D1HwndRenderTarget* renderTarget)
{
	/*bg = ImageManager::GetInstance()->AddImage("�ε�_1",
		L"image/loading.bmp", 852, 480);*/
	renderTarget->CreateCompatibleRenderTarget(&loadingRT);
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

	// Draw
	loadingRT->BeginDraw();
	// loadingScene ���� ����


	loadingRT->EndDraw();

	ComPtr<ID2D1Bitmap> result;
	loadingRT->GetBitmap(&result);
	renderTarget->DrawBitmap(result.Get());
}
