#include "GameManager.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "../MapTool/EditorTile.h"
#include "Tile.h"
#include "ObjectManager.h"
#include "ObjectFactory.h"

using json = nlohmann::json;

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

void GameManager::LoadObject(const char* path)
{
	std::ifstream in(path);
	if (!in.is_open())
	{
		printf("Failed to open: %s\n", path);
		return;
	}

	json j;
	in >> j;
	in.close();

	for (const auto& item : j)
	{
		std::string name = item.value("name", "");
		float x = item.value("x", 0.0f);
		float y = item.value("y", 0.0f);
		float w = item.value("width", 0.0f);
		float h = item.value("height", 0.0f);

		// 생성
		GameObject* obj = ObjectFactory::Get().Create(name);
		if (!obj)
		{
			int a = 10;
			continue;
		}

		const float tileSize = 128.f; // 툴과 동일한 타일 단위
		float gx = x / tileSize;
		float gy = y / tileSize;

		float gameTileSize = 64.f;
		// 게임 좌표계로 변환 (필요시 다시 픽셀로 곱하거나 절대 좌표 계산)
		FPOINT worldPos = { gx * gameTileSize, gy * gameTileSize };
		obj->SetPos(worldPos);

		// 월드에 추가
		ObjectManager::GetInstance()->AddObject(RENDERORDER::RENDER_ITEM, obj);
	}

	printf("Loaded objects from: %s\n", path);
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
	
	//TODO 임시
	LoadObject("Data/map1.json");
}

bool GameManager::IsTileValid(int x, int y)
{
	if (x < 0 || x >= 16 || y < 0 || y >= 16)
		return false;

	if (tileMap[y][x] == nullptr)
		return false;

	return tileMap[y][x]->IsValid();
}