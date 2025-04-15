#include "YongScene.h"
#include "Image.h"
#include "TestAnimationObject.h"
#include "TestRenderSort.h"
#include "TestYongSceneBackGround.h"

HRESULT YongScene::Init(ID2D1HwndRenderTarget* renderTarget)
{

    ImageManager::GetInstance()->AddImage(
        "TestJunYongAttack", L"Image/TestJunYongAttack.bmp", renderTarget);

    ImageManager::GetInstance()->AddImage(
        "TestJunyongWalk", L"Image/TestJunyongWalk.bmp", renderTarget);

    ImageManager::GetInstance()->AddImage(
        "monstersbig03", L"Textures/monstersbig03.png.", renderTarget);

    //Test = new TestAnimationObject();
    //Test->Init();
    ObjectManager::GetInstance()->AddObject(RENDER_PLAYER, new TestAnimationObject());
    ObjectManager::GetInstance()->AddObject(RENDER_MONSTER, new TestRenderSort());
    ObjectManager::GetInstance()->AddObject(RENDER_BACKGROUND, new TestYongSceneBackGround());
    
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
    //Test->Update(TimeDelta);
}

void YongScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
   // backGround->Render(renderTarget, 0, 0);
   // Test->Render(renderTarget);
}
