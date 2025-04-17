#include "GameManager.h"

#include "../MapTool/EditorTile.h"
#include "Tile.h"

void GameManager::Release()
{
	ReleaseInstance();
}

void GameManager::LoadTile(const char* path)
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

			ObjectManager::GetInstance()->AddObject(RENDERORDER::RENDER_TILE, tile);

			float renderX = (src.pos.x + 0.5f) * GAME_TILE_SIZE;
			float renderY = (src.pos.y + 0.5f) * GAME_TILE_SIZE;

			tile->InitTile(src.atlasX, src.atlasY, src.valid, { renderX , renderY });

			tileMap[y][x] = tile;
		}
	}
}

void GameManager::GenerateDecoTile()
{
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{

			//상 하 좌 우에 인접한 타일이 없다면.
			//데코타일 추가

			//순서를 지켜야한다. 좌 우 데코타일은 위쪽 데코타일에 의해 가려져야 하니까.
			//맵 외곽에 대한 처리도 들어가야한다.

			//좌
			if (IsTileValid(x - 1, y) == false)
			{
				bool hasTileAbove = !IsTileValid(x, y - 1);
				tileMap[y][x]->CreateDecoTile(DecoDirection::LEFT, hasTileAbove);
			}

			//우
			if (IsTileValid(x + 1, y) == false)
			{
				bool hasTileAbove = !IsTileValid(x, y - 1);
				tileMap[y][x]->CreateDecoTile(DecoDirection::RIGHT, hasTileAbove);
			}

			//상
			if (IsTileValid(x, y - 1) == false)
			{
				tileMap[y][x]->CreateDecoTile(DecoDirection::TOP);
			}

			//하
			if (IsTileValid(x, y + 1) == false)
			{
				tileMap[y][x]->CreateDecoTile(DecoDirection::DOWN);
			}

		}
	}
}

void GameManager::Init(const char* path)
{
	LoadTile(path);
	GenerateDecoTile();
}

bool GameManager::IsTileValid(int x, int y)
{
	if (x < 0 || x >= 16 || y < 0 || y >= 16)
		return false;

	if (tileMap[y][x] == nullptr)
		return false;

	return tileMap[y][x]->IsValid();
}