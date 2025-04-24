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
#include "ChangeCobwebBomb.h"
#include "IncreaseGold.h"
#include "TreasureChest.h"
#include "Ladder1.h"
#include "Ladder2.h"
#include "Ladder3.h"
#include "Ladder4.h"
#include "CameraManager.h"
#include "SnakeMonster.h"
#include "SkeletonMonster.h"
#include "BossMonster.h"
#include "playerHP_UI.h"
#include "playerGold_UI.h"
#include "goldBackground_UI.h"
#include "playerBomb_UI.h"
#include "playerRope_UI.h"
#include "sandGlass_UI.h"
#include "lightBulb_UI.h"
#include "Journal_1.h"
#include "dialogUI.h"
#include "ShopInteractUI.h"
#include "ShopKeeper.h"
#include "SkeletonItem.h"
#include "Chicken.h"
#include "IncreaseHP.h"
#include "ItemDialog.h"
#include "GhostJar.h"
#include "GameOverUI.h"

HRESULT GameScene::Init(ID2D1RenderTarget* renderTarget)
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
    ImageManager::GetInstance()->AddImage("Snake_Monster", L"Textures/Monster/monsters.png",16,16, renderTarget);
    ImageManager::GetInstance()->AddImage("Skeleton_Monster", L"Textures/Monster/monsters.png", 16, 16, renderTarget);
    ImageManager::GetInstance()->AddImage("Boss_Monster", L"Textures/Monster/boss.png", 8, 8, renderTarget);

    ImageManager::GetInstance()->AddImage("GameOver_journalRear", L"Textures/UI/Journal/journal_back.png", renderTarget);
    ImageManager::GetInstance()->AddImage("GameOver_journalFront", L"Textures/UI/Journal/journal_top_gameover.png", renderTarget);
    ImageManager::GetInstance()->AddImage("GameOver_journalSelectBox", L"Textures/UI/Journal/journal_select.png", renderTarget);
    ImageManager::GetInstance()->AddImage("GameOver_characterSticker", L"Textures/UI/Journal/journal_stickers/yellow.png", renderTarget);
    ImageManager::GetInstance()->AddImage("GameOver_journalBack", L"Textures/UI/journal/Journal_back.png", renderTarget);
    ImageManager::GetInstance()->AddImage("GameOver_journalClip", L"Textures/UI/Journal/journal_Clip.png", renderTarget);
    ImageManager::GetInstance()->AddImage("GameOverPage", L"Textures/UI/Journal/journal_back.png", renderTarget);
    ImageManager::GetInstance()->AddImage("playerLife", L"Textures/UI/Hud/playerLife.png", renderTarget);
    ImageManager::GetInstance()->AddImage("playerLifeRunsOut", L"Textures/UI/Hud/playerLIfeRunsOut.png", renderTarget);
    ImageManager::GetInstance()->AddImage("playerBombImage", L"Textures/UI/Hud/bomb.png", renderTarget);
    ImageManager::GetInstance()->AddImage("playerRopeImage", L"Textures/UI/Hud/rope.png", renderTarget);
    ImageManager::GetInstance()->AddImage("playerGoldImage", L"Textures/UI/Hud/currency.png", renderTarget);
    ImageManager::GetInstance()->AddImage("goldBackgroundImage", L"Textures/UI/Hud/currencyBackground.png", renderTarget);
    ImageManager::GetInstance()->AddImage("sandGlassImage", L"Textures/UI/Hud/sandGlass.png", renderTarget);
    ImageManager::GetInstance()->AddImage("lightBulbImage", L"Textures/UI/Hud/lightBulb.png", renderTarget);
    ImageManager::GetInstance()->AddImage("shopDialogBox", L"Textures/UI/Menu/shopDialogBox.png", renderTarget);
    ImageManager::GetInstance()->AddImage("shopKeeper", L"Textures/Entities/People/shopkeeper.png", 6, 7, renderTarget);

   // fx_big
    ImageManager::GetInstance()->AddImage("fx_big", L"Textures/fx_big.png", 4, 4, renderTarget);

    ImageManager::GetInstance()->AddImage("journalPage", L"Textures/UI/Journal/journal_pagetorn.png", renderTarget);
    ImageManager::GetInstance()->AddImage("journal_1_Photo", L"Textures/UI/Journal/journal_1_Photo.png", renderTarget);
    ImageManager::GetInstance()->AddImage("TextDialogBox", L"Textures/UI/Hud/TextDialogBox.png", renderTarget);
    ImageManager::GetInstance()->AddImage("TextDialogArrow", L"Textures/UI/Hud/TextDialogArrow.png", renderTarget);

  

    ImageManager::GetInstance()->AddImage("Trap", L"Textures/floormisc.png", renderTarget);

    ImageManager::GetInstance()->AddImage("Effect", L"Textures/fx_small.png", renderTarget);
    ImageManager::GetInstance()->AddImage("Rubble", L"Textures/fx_rubble.png", renderTarget);
    ImageManager::GetInstance()->AddImage("Explosion", L"Textures/fx_Explosion.png", 4, 4, renderTarget);
    ImageManager::GetInstance()->AddImage("turkey", L"Textures/Entities/Mounts/turkey.png", 8, 11, renderTarget);

    ImageManager::GetInstance()->AddImage("shopDialogBox", L"Textures/UI/Menu/shopDialogBox.png", renderTarget);
    ImageManager::GetInstance()->AddImage("currency_GameOver", L"Textures/UI/Hud/currency_GameOver.png", renderTarget);

    
    background = ImageManager::GetInstance()->AddImage("background", L"Textures/deco_cosmic.png", renderTarget);





    //GameManager::GetInstance()->Init("hongScene");


    CameraManager::GetInstance()->Init();
 
   // ObjectManager::GetInstance()->AddObject(RENDER_PLAYER, new TestAnimationObject());
    // ObjectManager::GetInstance()->AddObject(RENDER_MONSTER, new TestRenderSort());
    // ObjectManager::GetInstance()->AddObject(RENDER_BACKGROUND, new TestYongSceneBackGround());
    //ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new Gun());
    //ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new IncreaseBomb());
    //ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new IncreaseGold());
    ////ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new TreasureChest());
    //ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new Ladder1());
    //ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new Ladder2());
    //ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new Ladder3());
    //ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new Ladder4());
    //ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new ChangeCobwebBomb());


    //ObjectManager::GetInstance()->AddObject(RENDER_MONSTER, new SnakeMonster());
    //ObjectManager::GetInstance()->AddObject(RENDER_MONSTER, new SkeletonMonster());

    GameManager::GetInstance()->Init("hongScene");
    //GameManager::GetInstance()->Init("caveScene");

    ObjectManager::GetInstance()->AddObject(RENDER_UI, new playerHP_UI(renderTarget));
   // ObjectManager::GetInstance()->AddObject(RENDER_UI, new goldBackground_UI());
    ObjectManager::GetInstance()->AddObject(RENDER_UI, new playerGold_UI(renderTarget));
    ObjectManager::GetInstance()->AddObject(RENDER_UI, new sandGlass_UI(renderTarget));
    ObjectManager::GetInstance()->AddObject(RENDER_UI, new lightBulb_UI());
    ObjectManager::GetInstance()->AddObject(RENDER_UI, new playerRope_UI(renderTarget));
    ObjectManager::GetInstance()->AddObject(RENDER_UI, new playerBomb_UI(renderTarget));
    ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new IncreaseBomb());
    ObjectManager::GetInstance()->AddObject(RENDER_UI, new ShopInteractUI(renderTarget));
    ObjectManager::GetInstance()->AddObject(RENDER_NPC, new ShopKeeper());
    
  
    //ObjectManager::GetInstance()->AddObject(RENDER_UI, new dialogUI());+		[allocator]	allocator	std::_Compressed_pair<std::allocator<char>,std::_String_val<std::_Simple_types<char>>,1>
    ObjectManager::GetInstance()->AddObject(RENDER_UI, new GameOverUI(renderTarget));

    ObjectManager::GetInstance()->AddObject(RENDER_UI, new Journal_1());
    //ObjectManager::GetInstance()->AddObject(RENDER_MONSTER, new SnakeMonster());
    ObjectManager::GetInstance()->AddObject(RENDER_MONSTER, new SkeletonMonster());
    ObjectManager::GetInstance()->AddObject(RENDER_MONSTER, new BossMonster());

    ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new SkeletonItem());
    //ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new Chicken());
    ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new IncreaseHP());

    ObjectManager::GetInstance()->AddObject(RENDER_ITEM, new GhostJar());


   // GameManager::GetInstance()->Init("hongScene");

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
    CameraManager::GetInstance()->Update(TimeDelta);
}

void GameScene::LateUpdate(float TimeDelta)
{
}

void GameScene::Render(ID2D1RenderTarget* renderTarget)
{
    Viewport viewTile = CameraManager::GetInstance()->GetInRect();
   /* if (yellow)
        yellow->Render(renderTarget);*/

    RenderBackground(renderTarget);
}

void GameScene::RenderBackground(ID2D1RenderTarget* rt)
{
    float scale = GAME_TILE_SIZE / 128.f;  // 0.375
    float tileDrawSize = 512.f * scale;    // 하나의 배경 이미지가 차지할 실제 픽셀 크기 = 192

    int tilesX = static_cast<int>(ceil((40 * GAME_TILE_SIZE) / tileDrawSize)); // 1920 / 192 = 10
    int tilesY = static_cast<int>(ceil((32 * GAME_TILE_SIZE) / tileDrawSize)); // 1536 / 192 = 8

    for (int y = 0; y < 32; ++y)
    {
        for (int x = 0; x < 40; ++x)
        {
            float drawX = x * tileDrawSize * 0.5f;
            float drawY = y * tileDrawSize * 0.5f;
            FPOINT cameraPos = FPOINT(drawX, drawY) + CameraManager::GetInstance()->GetPos();

            const float margin = 128.f; // 여유 픽셀

            if (cameraPos.x + tileDrawSize < -margin || cameraPos.x > WINSIZE_X + margin ||
                cameraPos.y + tileDrawSize < -margin || cameraPos.y > WINSIZE_Y + margin)
            {
                continue; // 화면 바깥이면 그리지 않음
            }
            background->Render(rt, cameraPos.x, cameraPos.y, scale, scale, 0, 0, 512.f, 512.f);
        }
    }
}
