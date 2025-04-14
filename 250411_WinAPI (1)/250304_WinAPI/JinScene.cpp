#include "JinScene.h"
#include "Image.h"

HRESULT JinScene::Init()
{
    ImageManager::GetInstance()->AddImage("�ӽ� �̹���", TEXT("image/rocket.bmp"), 52, 64, true, RGB(255, 0, 255));
    tempImage = ImageManager::GetInstance()->FindImage("�ӽ� �̹���");
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

void JinScene::Render(HDC hdc)
{
    if (tempImage)
    {
        tempImage->Render(hdc, pos.x, pos.y);
    }
}
