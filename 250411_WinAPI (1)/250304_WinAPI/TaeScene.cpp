#include "TaeScene.h"
#include "Image.h"
#include "ImageManager.h"
#include "KeyManager.h"
#include "Character.h"

HRESULT TaeScene::Init(ID2D1HwndRenderTarget* renderTarget)
{
    backGround = ImageManager::GetInstance()->AddImage("Tae_Background", TEXT("Image/Character/background.bmp"), renderTarget);

    /*backGround = new Image();
    if (FAILED(backGround->Init(TEXT("Image/Character/background.bmp"), 2048, 2048)))
    {
        MessageBox(g_hWnd,
            TEXT("backGround.bmp 생성 실패"), TEXT("경고"), MB_OK);
        return E_FAIL;
    }*/

    yellow = new Character();

    if (yellow)
        yellow->Init();



    return S_OK;
}

void TaeScene::Release()
{
    if (yellow)
    {
        yellow->Release();
        delete yellow;
        yellow = nullptr;
    }
}

void TaeScene::Update(float TimeDelta)
{
    if (yellow)
        yellow->Update(TimeDelta);

}

void TaeScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
    backGround->Render(renderTarget, 0, 0);

    /*HPEN hPen = CreatePen(PS_SOLID, 10, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);


    MoveToEx(hdc, 0, 300, NULL);
    LineTo(hdc, 2048, 300);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);*/

    if (yellow)
        yellow->Render(renderTarget);

}


