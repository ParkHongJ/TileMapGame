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

	// 1. �ӽ� ���ۿ� ��ü�� ���� �о����
	EditorTile buffer[16 * 16];
	fread(buffer, sizeof(EditorTile), 16 * 16, fp);
	fclose(fp);

	// 2. 2D tileMap�� �ɰ��� ����
	for (int y = 0; y < 16; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			const EditorTile& src = buffer[y * 16 + x];

			// ���ϴ� ���� ����
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

		// ����
		GameObject* obj = ObjectFactory::Get().Create(name);
		if (!obj)
		{
			int a = 10;
			continue;
		}

		const float tileSize = 128.f; // ���� ������ Ÿ�� ����
		float gx = x / tileSize;
		float gy = y / tileSize;

		float gameTileSize = 64.f;
		// ���� ��ǥ��� ��ȯ (�ʿ�� �ٽ� �ȼ��� ���ϰų� ���� ��ǥ ���)
		FPOINT worldPos = { gx * gameTileSize, gy * gameTileSize };
		obj->SetPos(worldPos);

		// ���忡 �߰�
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

			//�� �� �� �쿡 ������ Ÿ���� ���ٸ�.
			//����Ÿ�� �߰�

			//������ ���Ѿ��Ѵ�. �� �� ����Ÿ���� ���� ����Ÿ�Ͽ� ���� �������� �ϴϱ�.
			//�� �ܰ��� ���� ó���� �����Ѵ�.

			//��
			if (IsTileValid(x - 1, y) == false)
			{
				bool hasTileAbove = !IsTileValid(x, y - 1);
				tileMap[y][x]->CreateDecoTile(DecoDirection::LEFT, hasTileAbove);
			}

			//��
			if (IsTileValid(x + 1, y) == false)
			{
				bool hasTileAbove = !IsTileValid(x, y - 1);
				tileMap[y][x]->CreateDecoTile(DecoDirection::RIGHT, hasTileAbove);
			}

			//��
			if (IsTileValid(x, y - 1) == false)
			{
				tileMap[y][x]->CreateDecoTile(DecoDirection::TOP);
			}

			//��
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
	
	//TODO �ӽ�
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