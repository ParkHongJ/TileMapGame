#include "YongScene.h"
#include "Image.h"
#include "TestAnimationObject.h"

HRESULT YongScene::Init(ID2D1HwndRenderTarget* renderTarget)
{
    Test = new TestAnimationObject();
    Test->Init();

    backGround = ImageManager::GetInstance()->FindImage("BackBuffer");
    return S_OK;
}

void YongScene::Release()
{
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

void YongScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
    backGround->Render(renderTarget, 0, 0);
    Test->Render(renderTarget);
}
