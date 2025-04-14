#include "HongScene.h"
#include "ObjectManager.h"
#include "Tile.h"
#include "../MapTool/EditorTile.h"

void HongScene::LoadTile(const char* path)
{
	FILE* fp = fopen(path, "rb");
	if (!fp) return;

	// 1. 임시 버퍼에 전체를 먼저 읽어오기
	EditorTile buffer[16 * 16];
	fread(buffer, sizeof(EditorTile), 16 * 16, fp);
	fclose(fp);

	// 2. 2D tileMap에 쪼개서 삽입
	for (int y = 0; y < 16; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			const EditorTile& src = buffer[y * 16 + x];

			// 원하는 가공 삽입
			Tile* tile = new Tile;
			tile->Init();

			float renderX = (src.pos.x + 0.5f) * GAME_TILE_SIZE;
			float renderY = (src.pos.y + 0.5f) * GAME_TILE_SIZE;

			tile->InitTile(src.atlasX, src.atlasY, src.valid, { renderX , renderY });

			tileMap[y][x] = tile;
			//tileMap[y][x].atlasX = src.atlasX;
			//tileMap[y][x].atlasY = src.atlasY;
			//tileMap[y][x].valid = src.valid;

			// posX, posY를 자동으로 갱신
			//tileMap[y][x].posX = x;
			//tileMap[y][x].posY = y;
		}
	}
}

HRESULT HongScene::Init(ID2D1HwndRenderTarget* renderTarget)
{
	ObjectManager::GetInstance()->Init();
	ImageManager::GetInstance()->AddImage("CaveTile", L"Textures/CaveTile.png", renderTarget);
	LoadTile("Data/map1.tilemap");
	return S_OK;
}

void HongScene::Release()
{
	ObjectManager::GetInstance()->Release();
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			if (tileMap[y][x])
			{
				tileMap[y][x]->Release();
				delete tileMap[y][x];
			}
		}
	}
}

void HongScene::Update(float TimeDelta)
{
	ObjectManager::GetInstance()->Update(TimeDelta);
}

void HongScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
	ObjectManager::GetInstance()->Render(renderTarget);

	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			if (tileMap[y][x])
			{
				tileMap[y][x]->Render(renderTarget);
			}
		}
	}
}
