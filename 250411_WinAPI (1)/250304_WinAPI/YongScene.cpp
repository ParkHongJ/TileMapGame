#include "YongScene.h"
#include "TestAnimationObject.h"
HRESULT YongScene::Init()
{
    Test = new TestAnimationObject();
    Test->Init();
    return S_OK;
}

void YongScene::Release()
{
    
}

void YongScene::Update(float TimeDelta)
{
    Test->Update(TimeDelta);
}

void YongScene::Render(HDC hdc)
{
    
}
