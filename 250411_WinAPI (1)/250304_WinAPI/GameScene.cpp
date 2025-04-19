#include "pch.h"
#include "GameScene.h"
#include "GameManager.h"
#include "TaeScene.h"
#include "Image.h"
#include "ImageManager.h"
#include "KeyManager.h"
#include "Character.h"
#include "TimerManager.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "ObjectManager.h"
#include "TestAnimationObject.h"
#include "TestRenderSort.h"
#include "TestYongSceneBackGround.h"
#include "Bomb.h"
#include "Gun.h"
#include "Whip.h"
#include "IncreaseBomb.h"
#include "CameraManager.h"
#include "snakeMonster.h"

HRESULT GameScene::Init(ID2D1HwndRenderTarget* renderTarget)
{
    ImageManager::GetInstance()->AddImage(
        "TestJunYongAttack", L"Image/TestJunYongAttack.bmp", 6, 1, renderTarget);

    ImageManager::GetInstance()->AddImage(
        "TestJunyongWalk", L"Image/TestJunyongWalk.bmp", 8, 1, renderTarget);

    ImageManager::GetInstance()->AddImage(
        "monstersbig03", L"Textures/monstersbig03.png.", renderTarget);

    ImageManager::GetInstance()->AddImage(
        "items", L"Textures/items.png.", 16, 16, renderTarget);

    ImageManager::GetInstance()->AddImage("CaveTile", L"Textures/CaveTile.png", renderTarget);
    ImageManager::GetInstance()->AddImage("CaveDecoDown", L"Textures/CaveDecoDown.png", renderTarget);
    ImageManager::GetInstance()->AddImage("CaveDecoTop", L"Textures/CaveDecoTop.png", renderTarget);
    ImageManager::GetInstance()->AddImage("CaveDecoRight", L"Textures/CaveDecoRight.png", renderTarget);
    ImageManager::GetInstance()->AddImage("Border", L"Textures/border_main.png", renderTarget);
    ImageManager::GetInstance()->AddImage("Snake_Monster", L"Image/monster.png",16,16, renderTarget);

    GameManager::GetInstance()->Init("map1");

    CameraManager::GetInstance()->Init();
 

    // Collider register
    snake = new SnakeMonster;
    if (snake)
    {
        snake->Init();
    }

   // ObjectManager::GetInstance()->AddObject(RENDER_PLAYER, new TestAnimationObject());
    // ObjectManager::GetInstance()->AddObject(RENDER_MONSTER, new TestRenderSort());
    // ObjectManager::GetInstance()->AddObject(RENDER_BACKGROUND, new TestYongSceneBackGround());
   /* ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new Gun());
    ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new Whip());
    ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new IncreaseBomb());*/

    ObjectManager::GetInstance()->AddObject(RENDER_MONSTER, new SnakeMonster());

    return S_OK;
}

void GameScene::Release()
{
    //if (yellow)
    //{
    //    yellow->Release();
    //    delete yellow;
    //    yellow = nullptr;
    //}
    //ObjectManager::GetInstance()->Release();

    //CollisionManager::GetInstance()->Release();
}

void GameScene::Update(float TimeDelta)
{
   /* if (yellow)
        yellow->Update(TimeDelta);*/
    if (snake)
        snake->Update(TimeDelta);

    CameraManager::GetInstance()->Update(TimeDelta);
}

void GameScene::LateUpdate(float TimeDelta)
{
}

void GameScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
    Viewport viewTile = CameraManager::GetInstance()->GetInRect();
   /* if (yellow)
        yellow->Render(renderTarget);*/

    if (snake)
        snake->Render(renderTarget);

}
