#include "JinScene.h"
#include "Image.h"

HRESULT JinScene::Init(ID2D1HwndRenderTarget* renderTarget)
{
    ImageManager::GetInstance()->AddImage("tempRocket", TEXT("image/rocket.bmp"), renderTarget);
    tempImage = ImageManager::GetInstance()->FindImage("tempRocket");
    if (!tempImage)
        return E_FAIL;
    pos.x = WINSIZE_X * 0.9;
    pos.y = WINSIZE_Y * 0.5;

    return S_OK;
}

void JinScene::Release()
{
}

void JinScene::Update(float TimeDelta)
{
}

void JinScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
    if (tempImage)
    {
        tempImage->Render(renderTarget, pos.x, pos.y);
    }
}
