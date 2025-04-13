#include "YongScene.h"
#include "Image.h"
#include "TestAnimationObject.h"

HRESULT YongScene::Init()
{
    Test = new TestAnimationObject();
    Test->Init();

    backGround = new Image();
    if (FAILED(backGround->Init(TEXT("Image/BackGround.bmp"), WINSIZE_X, WINSIZE_Y)))
    {
        MessageBox(g_hWnd,
            TEXT("Image/backGround.bmp 생성 실패"), TEXT("경고"), MB_OK);
        return E_FAIL;
    }

    return S_OK;
}

void YongScene::Release()
{
    if (backGround)
    {
        backGround->Release();
        delete backGround;
        backGround = nullptr;
    }

    if (Test)
    {
        Test->Release();
        delete Test;
        Test = nullptr;
    }
}

void YongScene::Update(float TimeDelta)
{
    Test->Update(TimeDelta);
}

void YongScene::Render(HDC hdc)
{
    backGround->Render(hdc);
}
