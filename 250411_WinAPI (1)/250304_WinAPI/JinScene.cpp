#include "pch.h"
#include "JinScene.h"
#include "Image.h"

//테스트용 캐릭터 Colin
HRESULT JinScene::Init(ID2D1RenderTarget* renderTarget)
{
    ImageManager::GetInstance()->AddImage("tempColin", TEXT("image/character/char_Colin/Colin.png"), renderTarget);
    tempImage = ImageManager::GetInstance()->FindImage("tempColin");
    if (!tempImage)
        return E_FAIL;
    pos.x = WINSIZE_X * 0.9;
    pos.y = WINSIZE_Y * 0.5;

    life = 1;
    bomb = 3;
    rope = 2;
    currency = 3000;
    isAlive = true;

    return S_OK;
}

void JinScene::Release()
{
}

void JinScene::Update(float TimeDelta)
{
    if (KeyManager::GetInstance()->IsOnceKeyDown('T'))
        life--;
}

void JinScene::Render(ID2D1RenderTarget* renderTarget)
{
    if (tempImage)
    {
        tempImage->Render(renderTarget, pos.x, pos.y);
    }
}
