#include "pch.h"
#include "HongScene.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "DummyHongPlayer.h"
#include "Image.h"
#include "GameManager.h"
#include "CameraManager.h"
#include "ArrowTrap.h"
#include "NiddleTrap.h"
#include "HeavyBlock.h"

HRESULT HongScene::Init(ID2D1RenderTarget* renderTarget)
{
	CollisionManager::GetInstance()->Init();
	CollisionManager::GetInstance()->Clear();

	ObjectManager::GetInstance()->ClearObjects();

	ImageManager::GetInstance()->AddImage("CaveTile", L"Textures/CaveTile.png", renderTarget);
	ImageManager::GetInstance()->AddImage("floor_cave", L"Textures/floor_cave.png", renderTarget);
	ImageManager::GetInstance()->AddImage("CaveDecoDown", L"Textures/CaveDecoDown.png", renderTarget);
	ImageManager::GetInstance()->AddImage("CaveDecoTop", L"Textures/CaveDecoTop.png", renderTarget);
	ImageManager::GetInstance()->AddImage("CaveDecoRight", L"Textures/CaveDecoRight.png", renderTarget);
	ImageManager::GetInstance()->AddImage("Border", L"Textures/border_main.png", renderTarget);
	ImageManager::GetInstance()->AddImage("Effect", L"Textures/fx_small.png", renderTarget);
	ImageManager::GetInstance()->AddImage("Rubble", L"Textures/fx_rubble.png", renderTarget);
	ImageManager::GetInstance()->AddImage("Explosion", L"Textures/fx_Explosion.png", 4, 4, renderTarget);
	ImageManager::GetInstance()->AddImage("Trap", L"Textures/floormisc.png", renderTarget);
	ImageManager::GetInstance()->AddImage("items", L"Textures/items.png", renderTarget);
	background = ImageManager::GetInstance()->AddImage("background", L"Textures/deco_cosmic.png", renderTarget);

	GameManager::GetInstance()->Init("caveScene");

	//ObjectManager::GetInstance()->AddObject(RENDERORDER::RENDER_PLAYER, new DummyHongPlayer);

	//ObjectManager::GetInstance()->AddObject(RENDER_PLAYER, new NiddleTrap);
	ObjectManager::GetInstance()->AddObject(RENDER_PLAYER, new HeavyBlock);
	return S_OK;
}

void HongScene::Release()
{
	//ObjectManager::GetInstance()->Release();
	CollisionManager::GetInstance()->Release();
}

void HongScene::Update(float TimeDelta)
{
	ObjectManager* objMgr =  ObjectManager::GetInstance();//->Update(TimeDelta);
	int a = 10;
	//CollisionManager::GetInstance()->Update(TimeDelta);

	CameraManager::GetInstance()->Update(TimeDelta);
}

void HongScene::LateUpdate(float TimeDelta)
{
	//ObjectManager::GetInstance()->LateUpdate(TimeDelta);
}

void HongScene::Render(ID2D1RenderTarget* renderTarget)
{
	//ObjectManager::GetInstance()->Render(renderTarget);
	
	/*for (int i = 0; i < 20; i++)
	{
		background->Render(renderTarget, 0, 0, scale, scale, 0, 0, 512.f, 512.f);
	}*/

	 
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
			background->Render(renderTarget, cameraPos.x, cameraPos.y, scale, scale, 0, 0, 512.f, 512.f);
		}
	}
#ifdef _DEBUG
	//CollisionManager::GetInstance()->DebugRender(renderTarget);
#endif _DEBUG
}
