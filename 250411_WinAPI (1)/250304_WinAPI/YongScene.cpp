#include "pch.h"
#include "YongScene.h"
#include "Image.h"
#include "TestAnimationObject.h"
#include "TestRenderSort.h"
#include "TestYongSceneBackGround.h"
#include "Bomb.h"
#include "Gun.h"
#include "Whip.h"
#include "IncreaseBomb.h"
#include "Rope.h"
#include "RopeController.h"
#include "ImageManager.h"

HRESULT YongScene::Init(ID2D1RenderTarget* renderTarget)
{

    ImageManager::GetInstance()->AddImage(
        "TestJunYongAttack", L"Image/TestJunYongAttack.bmp", 6, 1, renderTarget);

    ImageManager::GetInstance()->AddImage(
        "TestJunyongWalk", L"Image/TestJunyongWalk.bmp",8, 1, renderTarget);

    ImageManager::GetInstance()->AddImage(
        "monstersbig03", L"Textures/monstersbig03.png.", renderTarget);

    ImageManager::GetInstance()->AddImage(
        "items", L"Textures/items.png.", 16, 16, renderTarget);

    //Test = new TestAnimationObject();
    //Test->Init();
   // ObjectManager::GetInstance()->AddObject(RENDER_PLAYER, new TestAnimationObject());
   //// ObjectManager::GetInstance()->AddObject(RENDER_MONSTER, new TestRenderSort());
   //// ObjectManager::GetInstance()->AddObject(RENDER_BACKGROUND, new TestYongSceneBackGround());
   // ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new Gun());
   // ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new Whip());
   // ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new IncreaseBomb());

    //ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new Rope());
    ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new RopeController());
    
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

void YongScene::Render(ID2D1RenderTarget* renderTarget)
{
   // backGround->Render(renderTarget, 0, 0);
   // Test->Render(renderTarget);
}
