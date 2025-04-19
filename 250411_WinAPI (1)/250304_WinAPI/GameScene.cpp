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
#include "IncreaseGold.h"
#include "TreasureChest.h"
#include "Ladder.h"
#include "CameraManager.h"
#include "snakeMonster.h"
#include "playerHP_UI.h"
#include "playerGold_UI.h"
#include "goldBackground_UI.h"
#include "playerBomb_UI.h"
#include "playerRope_UI.h"
#include "sandGlass_UI.h"
#include "lightBulb_UI.h"
#include "Journal_1.h"
#include "dialogUI.h"

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

    ImageManager::GetInstance()->AddImage("floor_cave", L"Textures/floor_cave.png", 12, 12, renderTarget);


    ImageManager::GetInstance()->AddImage("CaveTile", L"Textures/CaveTile.png", renderTarget);
    ImageManager::GetInstance()->AddImage("CaveDecoDown", L"Textures/CaveDecoDown.png", renderTarget);
    ImageManager::GetInstance()->AddImage("CaveDecoTop", L"Textures/CaveDecoTop.png", renderTarget);
    ImageManager::GetInstance()->AddImage("CaveDecoRight", L"Textures/CaveDecoRight.png", renderTarget);
    ImageManager::GetInstance()->AddImage("Border", L"Textures/border_main.png", renderTarget);
    ImageManager::GetInstance()->AddImage("Snake_Monster", L"Image/monster.png",16,16, renderTarget);
    ImageManager::GetInstance()->AddImage("GameOverPage", L"Textures/UI/Journal/journal_back.png", renderTarget);
    ImageManager::GetInstance()->AddImage("playerLife", L"Textures/UI/Hud/playerLife.png", renderTarget);
    ImageManager::GetInstance()->AddImage("playerLifeRunsOut", L"Textures/UI/Hud/playerLIfeRunsOut.png", renderTarget);
    ImageManager::GetInstance()->AddImage("playerBombImage", L"Textures/UI/Hud/bomb.png", renderTarget);
    ImageManager::GetInstance()->AddImage("playerRopeImage", L"Textures/UI/Hud/rope.png", renderTarget);
    ImageManager::GetInstance()->AddImage("playerGoldImage", L"Textures/UI/Hud/currency.png", renderTarget);
    ImageManager::GetInstance()->AddImage("goldBackgroundImage", L"Textures/UI/Hud/currencyBackground.png", renderTarget);
    ImageManager::GetInstance()->AddImage("sandGlassImage", L"Textures/UI/Hud/sandGlass.png", renderTarget);
    ImageManager::GetInstance()->AddImage("lightBulbImage", L"Textures/UI/Hud/lightBulb.png", renderTarget);

   // fx_big
    ImageManager::GetInstance()->AddImage("fx_big", L"Textures/fx_big.png", 4, 4, renderTarget);

    ImageManager::GetInstance()->AddImage("journalPage", L"Textures/UI/Journal/journal_pagetorn.png", renderTarget);
    ImageManager::GetInstance()->AddImage("journal_1_Photo", L"Textures/UI/Journal/journal_1_Photo.png", renderTarget);
    ImageManager::GetInstance()->AddImage("TextDialogBox", L"Textures/UI/Hud/TextDialogBox.png", renderTarget);
    ImageManager::GetInstance()->AddImage("TextDialogArrow", L"Textures/UI/Hud/TextDialogArrow.png", renderTarget);

    GameManager::GetInstance()->Init("map1");
  

    GameManager::GetInstance()->Init("Data/map1.tilemap");

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
    ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new Gun());
   ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new Whip());
    ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new IncreaseBomb());
    ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new IncreaseGold());
    ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new TreasureChest());
    ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new Ladder());

    ObjectManager::GetInstance()->AddObject(RENDER_UI, new playerHP_UI());
    ObjectManager::GetInstance()->AddObject(RENDER_UI, new playerGold_UI());
    ObjectManager::GetInstance()->AddObject(RENDER_UI, new goldBackground_UI());
    ObjectManager::GetInstance()->AddObject(RENDER_UI, new sandGlass_UI());
    ObjectManager::GetInstance()->AddObject(RENDER_UI, new lightBulb_UI());
    ObjectManager::GetInstance()->AddObject(RENDER_UI, new playerRope_UI());
    ObjectManager::GetInstance()->AddObject(RENDER_UI, new playerBomb_UI());
    ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new IncreaseBomb());

    ObjectManager::GetInstance()->AddObject(RENDER_UI, new dialogUI());
    ObjectManager::GetInstance()->AddObject(RENDER_UI, new Journal_1());
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
